#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#define CATCH_CONFIG_PREFIX_ALL
#include <catch2/catch_test_macros.hpp>

#include "calebrjc/net/net.hpp"
#include "calebrjc/net/detail/getaddrinfo.hpp"

using namespace calebrjc;

/// @brief Return true if the data in rr is equal to the data in ai, and false otherwise.
/// @param rr The net::ResolveResult to be compared.
/// @param ai The addrinfo * to be compared.
/// @return True if the data in rr is equal to the data in ai, and false otherwise.
bool rr_ai_compare(net::resolve_result rr, addrinfo *ai) {
    // Equal if both are empty...
    if (!ai && rr.empty()) return true;

    // ... but not if only one is empty.
    if (!ai || rr.empty()) return false;

    // Compare the data from both lists
    for (auto &endpoint : rr) {
        if (memcmp(endpoint.data(), ai->ai_addr, ai->ai_addrlen) != 0) return false;
        ai = ai->ai_next;
    }

    return true;
}

CATCH_TEST_CASE("[net::resolve] Local name resolution (INADDR_ANY)", "[net]") {
    std::string name = net::any_address;
    std::string service("5000");

    auto endpoints = net::resolve(name, service);

    CATCH_SECTION("Resolution returns results") {
        CATCH_REQUIRE_FALSE(endpoints.empty());
    }

    CATCH_SECTION("Resolution is consistent with bare getaddrinfo()") {
        auto *server_info = net::detail::gai::getaddrinfo(name, service);

        CATCH_REQUIRE(rr_ai_compare(endpoints, server_info));

        freeaddrinfo(server_info);
    }
}

CATCH_TEST_CASE("[net::resolve] Local name resolution (INADDR_LOOPBACK)", "[net]") {
    std::string name = net::loopback_address;
    std::string service("5000");

    auto endpoints = net::resolve(name, service);

    CATCH_SECTION("Resolution returns results") {
        CATCH_REQUIRE_FALSE(endpoints.empty());
    }

    CATCH_SECTION("Resolution is consistent with bare getaddrinfo()") {
        auto *server_info = net::detail::gai::getaddrinfo(name, service);

        CATCH_REQUIRE(rr_ai_compare(endpoints, server_info));

        freeaddrinfo(server_info);
    }
}

CATCH_TEST_CASE("[net::resolve] Remote name resolution", "[net]") {
    std::string name("google.com");
    std::string service("80");

    auto endpoints = net::resolve(name, service);

    CATCH_SECTION("Resolution returns results") {
        CATCH_REQUIRE_FALSE(endpoints.empty());
    }

    CATCH_SECTION("Resolution is consistent with bare getaddrinfo()") {
        auto *remote_info = net::detail::gai::getaddrinfo(name, service);

        CATCH_REQUIRE(rr_ai_compare(endpoints, remote_info));

        freeaddrinfo(remote_info);
    }
}
