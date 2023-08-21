#include "calebrjc/net/connection.hpp"

#define CATCH_CONFIG_PREFIX_ALL
#include <catch2/catch_test_macros.hpp>

using namespace calebrjc;

static const std::string hostname("tcpbin.com");
static const std::string service("4242");
static const net::buffer message("Hello!\n");

CATCH_TEST_CASE("[net::connection] Initial state is correct", "[net]") {
    net::connection conn;
    std::error_code ec;
    
    // The connection should not be connected...
    CATCH_REQUIRE_FALSE(conn.is_connected());

    // ... and should fail to send data...
    conn.send(message, ec);
    CATCH_REQUIRE(ec);

    // ... and should fail to receive data...
    auto buffer = conn.receive(ec);
    CATCH_REQUIRE(ec);

    // ... and should not be bound to any valid local endpoint...
    CATCH_REQUIRE(conn.local_endpoint() == net::endpoint());

    // ... and should not be connected to any valid remote endpoint...
    CATCH_REQUIRE(conn.remote_endpoint() == net::endpoint());
}

CATCH_TEST_CASE("[net::connection] Connection cannot be opened with invalid endpoints", "[net]") {
    net::connection conn;
    std::error_code ec;

    conn.connect(net::resolve_result(), ec);

    // An error should have been reported...
    CATCH_REQUIRE(ec);
    
    // The connection should not be connected...
    CATCH_REQUIRE_FALSE(conn.is_connected());

    // ... and should fail to send data...
    conn.send(message, ec);
    CATCH_REQUIRE(ec);

    // ... and should fail to receive data...
    auto buffer = conn.receive(ec);
    CATCH_REQUIRE(ec);

    // ... and should not be bound to any valid local endpoint...
    CATCH_REQUIRE(conn.local_endpoint() == net::endpoint());

    // ... and should not be connected to any valid remote endpoint...
    CATCH_REQUIRE(conn.remote_endpoint() == net::endpoint());
}

CATCH_TEST_CASE("[net::connection] Connection can be opened with valid endpoints", "[net]") {
    net::connection conn;
    std::error_code ec;

    auto endpoints = net::resolve(hostname, service);
    conn.connect(endpoints, ec);

    // An error should have been reported...
    CATCH_REQUIRE_FALSE(ec);
    
    // The connection should not be connected...
    CATCH_REQUIRE(conn.is_connected());

    // ... and should not be bound to any valid local endpoint...
    CATCH_REQUIRE_FALSE(conn.local_endpoint() == net::endpoint());

    // ... and should not be connected to any valid remote endpoint...
    CATCH_REQUIRE_FALSE(conn.remote_endpoint() == net::endpoint());

    CATCH_SECTION("Functional test") {
        conn.send(message, ec);
        CATCH_REQUIRE_FALSE(ec);

        auto buffer = conn.receive(ec);
        CATCH_REQUIRE_FALSE(ec);
        CATCH_REQUIRE(buffer.size() > 0);
    }
}
