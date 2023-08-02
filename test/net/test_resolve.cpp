#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#define CATCH_CONFIG_PREFIX_ALL
#include <catch2/catch_test_macros.hpp>

#include "calebrjc/net/net.hpp"

using namespace calebrjc;

/// @brief Return the addrinfo * from a call to getaddrinfo() with the name and service specified.
/// The pointer will have to be freed with freeaddrinfo().
/// @param name The name to be resolved.
/// @param service The service to be resolved.
/// @param is_binding_address True if the returned structure should contain addresses suitable for
/// binding, and false otherwise (see: AI_PASSIVE).
/// @return The addrinfo * from a call to getaddrinfo() with the name and service specified.
addrinfo *bare_getaddrinfo(std::string name, std::string service, bool is_binding_address) {
    // Get remote address info
    addrinfo hints;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_ADDRCONFIG | (is_binding_address) ? AI_PASSIVE : 0;

    addrinfo *target_info;
    const char *gai_name = (is_binding_address) ? NULL : name.c_str();
    int gai_result       = getaddrinfo(gai_name, service.c_str(), &hints, &target_info);
    if (gai_result != 0) return NULL;

    return target_info;
}

/// @brief Return true if the data in rr is equal to the data in ai, and false otherwise.
/// @param rr The net::ResolveResult to be compared.
/// @param ai The addrinfo * to be compared.
/// @return True if the data in rr is equal to the data in ai, and false otherwise.
bool rr_ai_compare(net::ResolveResult rr, addrinfo *ai) {
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

CATCH_TEST_CASE("[net::resolve] Local name resolution", "[net]") {
    std::string name = net::all_local_interfaces;
    std::string service("5000");

    auto endpoints = net::resolve(name, service);

    CATCH_SECTION("Resolution returns results") {
        CATCH_REQUIRE_FALSE(endpoints.empty());
    }

    CATCH_SECTION("Resolution is consistent with bare getaddrinfo()") {
        auto *server_info = bare_getaddrinfo(name, service, true);

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
        auto *remote_info = bare_getaddrinfo(name, service, false);

        CATCH_REQUIRE(rr_ai_compare(endpoints, remote_info));

        freeaddrinfo(remote_info);
    }
}
