#include "calebrjc/net/detail/socket_ops.hpp"

#define CATCH_CONFIG_PREFIX_ALL
#include <catch2/catch_test_macros.hpp>

#include "calebrjc/net/addresses.hpp"
#include "calebrjc/net/resolve.hpp"

using namespace calebrjc;

CATCH_TEST_CASE("[net::detail::socket_ops] create_listening_socket()") {
    std::string hostname = net::any_address;
    std::string service("5000");

    auto rr               = net::resolve(hostname, service);
    socket_type socket_fd = net::detail::socket_ops::create_listening_socket(rr, 128, true);

    CATCH_REQUIRE(socket_fd != 0);
}
