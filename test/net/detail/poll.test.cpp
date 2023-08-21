#include "calebrjc/net/detail/poll.hpp"

#include <thread>

#define CATCH_CONFIG_PREFIX_ALL
#include <catch2/catch_test_macros.hpp>

#include "../test_utils/test_utils.hpp"
#include "calebrjc/net/addresses.hpp"
#include "calebrjc/net/connection.hpp"
#include "calebrjc/net/detail/socket_ops.hpp"

using namespace calebrjc;

static const std::string remote_hostname("tcpbin.com");
static const std::string remote_service("4242");
static const net::resolve_result remote_endpoint = net::resolve(remote_hostname, remote_service);

static const std::string local_hostname = net::loopback_address;
static const std::string local_service("5000");
static const net::resolve_result local_endpoint = net::resolve(local_hostname, local_service);

CATCH_TEST_CASE("[net::detail::poll] poll_socket()", "[net]") {
    std::error_code ec;
    net::resolve_result endpoints;
    socket_type socket_fd;

    CATCH_SECTION("Connected socket test") {
        // Create the connected socket
        socket_fd = net::detail::socket_ops::create_connected_socket(remote_endpoint);

        auto status = net::detail::poll::poll_socket(socket_fd);

        using namespace net::detail::poll;

        // The socket should be writable...
        CATCH_REQUIRE((status & socket_status::writable).bits());

        // ... and not have any waiting data to read...
        CATCH_REQUIRE_FALSE((status & socket_status::readable).bits());

        // ... and not be in an error state...
        CATCH_REQUIRE_FALSE((status & socket_status::error).bits());

        // .. and be currently connected.
        CATCH_REQUIRE_FALSE((status & socket_status::hung_up).bits());

        net::detail::socket_ops::close_socket(socket_fd);
    }

    CATCH_SECTION("Listening socket test") {
        // Start sim client thread
        sim_client_state scs;
        auto client_thread =
            std::thread([&] { simulate_incoming_connection(local_hostname, local_service, scs); });

        // Set up acceptor socket
        socket_fd = net::detail::socket_ops::create_listening_socket(local_endpoint, 128, true);
        scs.server_ready = true;

        while (!scs.client_ready) {}  // Wait for the client to attempt to connect
        net::detail::poll::poll_socket(socket_fd);
        scs.server_accepted_client = true;

        using namespace net::detail::poll;

        auto status = net::detail::poll::poll_socket(socket_fd);

        // The socket should not be writable...
        CATCH_REQUIRE_FALSE((status & socket_status::writable).bits());

        // ... and have a waiting connection (be readable)
        CATCH_REQUIRE((status & socket_status::readable).bits());

        // ... and not be in an error state...
        CATCH_REQUIRE_FALSE((status & socket_status::error).bits());

        // .. and be currently connected.
        CATCH_REQUIRE_FALSE((status & socket_status::hung_up).bits());

        scs.server_done = true;
        if (client_thread.joinable()) client_thread.join();
    }
}

CATCH_TEST_CASE("[net::detail::poll] poll_group", "[net]") {
    net::detail::poll::poll_group pg;
    std::error_code ec;

    net::connection connection_1;
    net::connection connection_2;

    // Initial state -------------------------------------------------------------------------------
    CATCH_REQUIRE(pg.size() == 0);
    // ---------------------------------------------------------------------------------------------

    connection_1.connect(remote_endpoint, ec);
    CATCH_REQUIRE_FALSE(ec);

    connection_2.connect(remote_endpoint, ec);
    CATCH_REQUIRE_FALSE(ec);

    // Adding sockets increases size ---------------------------------------------------------------
    pg.add_socket(connection_1.native_socket());
    pg.add_socket(connection_2.native_socket());

    CATCH_REQUIRE(pg.size() == 2);
    // ---------------------------------------------------------------------------------------------

    connection_1.send(net::buffer("Hello!\n"), ec);
    CATCH_REQUIRE_FALSE(ec);

    // Wait until the connection has has data waiting to be read (archaic workaround method)
    while ((connection_1.receive(ec, net::receive_flags::peek)).is_empty()) {}

    // poll() returns expected results -------------------------------------------------------------
    auto pr = pg.poll();
    CATCH_REQUIRE(pr.size() == 2);

    for (const auto &[fd, status] : pr) {
        using namespace net::detail::poll;

        if (fd == connection_1.native_socket()) {
            // Should have data waiting to recv
            CATCH_REQUIRE(status & socket_status::readable);

            // Should be able to send data
            CATCH_REQUIRE(status & socket_status::writable);

            // No error or hang up
            CATCH_REQUIRE_FALSE(status & socket_status::error);
            CATCH_REQUIRE_FALSE(status & socket_status::hung_up);

            continue;
        }

        if (fd == connection_2.native_socket()) {
            // Should not have data waiting to recv
            CATCH_REQUIRE_FALSE(status & socket_status::readable);

            // Should be able to send data
            CATCH_REQUIRE(status & socket_status::writable);

            // No error or hang ups
            CATCH_REQUIRE_FALSE(status & socket_status::error);
            CATCH_REQUIRE_FALSE(status & socket_status::hung_up);

            continue;
        }

        CATCH_REQUIRE(0);
    }
    //----------------------------------------------------------------------------------------------

    // Removing sockets decreases size -------------------------------------------------------------
    pg.remove_socket(connection_1.native_socket());
    pg.remove_socket(connection_2.native_socket());

    CATCH_REQUIRE(pg.size() == 0);
    // ---------------------------------------------------------------------------------------------
}
