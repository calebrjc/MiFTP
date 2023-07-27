#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <catch2/catch_test_macros.hpp>

#include "calebrjc/net/net.hpp"

using namespace calebrjc;

TEST_CASE("[net::resolve] Local name resolution", "[net]") {
    std::string port_number("5000");
    auto endpoints = net::resolve(net::all_local_addresses, port_number);

    SECTION("Resolution returns results") {
        REQUIRE_FALSE(endpoints.empty());
    }

    SECTION("Resolution is consistent with bare getaddrinfo()") {
        // Get remote address info
        addrinfo hints;
        std::memset(&hints, 0, sizeof(hints));
        hints.ai_family   = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags    = AI_PASSIVE;

        addrinfo *server_info;
        int gai_result = getaddrinfo(NULL, port_number.c_str(), &hints, &server_info);
        if (gai_result != 0) { REQUIRE(0); }

        // Do all endpoints match all addrinfos?
        addrinfo *ai = server_info;
        for (auto &e : endpoints) {
            REQUIRE(memcmp(e.data(), ai->ai_addr, ai->ai_addrlen) == 0);
            ai = ai->ai_next;
        }

        freeaddrinfo(server_info);
    }
}

TEST_CASE("[net::resolve] Remote name resolution", "[net]") {
    std::string addr("google.com");
    std::string port_number("80");

    auto endpoints = net::resolve(addr, port_number);

    SECTION("Resolution returns results") {
        REQUIRE_FALSE(endpoints.empty());
    }

    SECTION("Resolution is consistent with bare getaddrinfo()") {
        // Prepare hints
        addrinfo hints;
        std::memset(&hints, 0, sizeof(hints));
        hints.ai_family   = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

        // Address resolution
        addrinfo *remote_info;
        int gai_result = getaddrinfo(addr.c_str(), port_number.c_str(), &hints, &remote_info);
        if (gai_result != 0) { REQUIRE(0); }

        // Do all endpoints match all addrinfos?
        addrinfo *ai = remote_info;
        for (auto &e : endpoints) {
            REQUIRE(memcmp(e.data(), ai->ai_addr, ai->ai_addrlen) == 0);
            ai = ai->ai_next;
        }
    }
}
