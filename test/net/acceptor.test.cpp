#include "calebrjc/net/acceptor.hpp"

#include <thread>

#define CATCH_CONFIG_PREFIX_ALL
#include <catch2/catch_test_macros.hpp>

#include "calebrjc/net/addresses.hpp"
#include "calebrjc/net/resolve.hpp"
#include "test_utils/test_utils.hpp"

using namespace calebrjc;

static const std::string service("5000");

CATCH_TEST_CASE("[net::acceptor] Initial state is correct", "[net]") {
    net::acceptor acceptor;
    std::error_code ec;

    // The acceptor should not be open...
    CATCH_REQUIRE_FALSE(acceptor.is_open());

    // ... should not have any pending connections...
    CATCH_REQUIRE_FALSE(acceptor.has_pending_connection());

    // ... or be bound to any valid local endpoint...
    CATCH_REQUIRE(acceptor.local_endpoint() == net::endpoint());

    // ... also, attempting to accept a connection should fail.
    auto conn = acceptor.accept(ec);
    CATCH_REQUIRE(ec);
}

CATCH_TEST_CASE("[net::acceptor] Acceptor cannot be opened with invalid endpoints", "[net]") {
    net::acceptor acceptor;
    std::error_code ec;

    acceptor.open(net::resolve_result(), ec, net::acceptor_config::reuse_address);

    // An error should have been reported...
    CATCH_REQUIRE(ec);

    // ... and, the acceptor should not be open...
    CATCH_REQUIRE_FALSE(acceptor.is_open());

    // .. should not have any pending connections...
    CATCH_REQUIRE_FALSE(acceptor.has_pending_connection());

    // ... or be bound to any valid local endpoint...
    CATCH_REQUIRE(acceptor.local_endpoint() == net::endpoint());

    // ... also, attempting to accept a connection should fail.
    auto conn = acceptor.accept(ec);
    CATCH_REQUIRE(ec);
}

CATCH_TEST_CASE("[net::acceptor] Acceptor can be opened with valid endpoints", "[net]") {
    net::acceptor acceptor;
    std::error_code ec;

    net::resolve_result endpoints = net::resolve(net::any_address, service);
    acceptor.open(endpoints, ec, net::acceptor_config::reuse_address);

    // An error should not have been reported...
    CATCH_REQUIRE_FALSE(ec);

    // ... and, the acceptor should be open...
    CATCH_REQUIRE(acceptor.is_open());

    // ... should not have any pending connections on open...
    CATCH_REQUIRE_FALSE(acceptor.has_pending_connection());

    // ... and should be bound to a valid local endpoint.
    CATCH_REQUIRE_FALSE(acceptor.local_endpoint() == net::endpoint());

    CATCH_SECTION("Functional test") {
        sim_client_state scs;
        auto client_thread =
            std::thread([&] { simulate_incoming_connection(net::loopback_address, service, scs); });

        scs.server_ready = true;
        while (!scs.client_ready) {}

        // The acceptor should have a pending connection
        CATCH_REQUIRE(acceptor.has_pending_connection());

        auto conn                  = acceptor.accept(ec);
        scs.server_accepted_client = true;

        // An error should not have been reported...
        CATCH_REQUIRE_FALSE(ec);

        // ... and the returned connection should be valid.
        CATCH_REQUIRE(conn.is_connected());

        scs.server_done = true;
        if (client_thread.joinable()) client_thread.join();
    }
}
