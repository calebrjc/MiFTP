#include <catch2/catch_test_macros.hpp>

#include "calebrjc/net/net.hpp"

using namespace calebrjc;

TEST_CASE("[net::resolve] Local name resolution", "[net]") {
    auto endpoints = net::resolve(net::local_host_name, "5000");

    SECTION("Resolution returns results") {
        REQUIRE_FALSE(endpoints.empty());
    }
}
