#include <future>
#include <sstream>

#define CATCH_CONFIG_PREFIX_ALL
#include <catch2/catch_test_macros.hpp>

#include "calebrjc/net/net.hpp"

using namespace calebrjc;

int run_server() {
    return std::system("./test_echo_server 3490");
}

int run_client() {
    return std::system("./test_echo_client localhost 3490");
}

CATCH_TEST_CASE("[net::everything_else] Client-server test", "[net]") {
    auto server_future = std::async(run_server);

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    auto client_future = std::async(run_client);

    server_future.wait();
    client_future.wait();

    CATCH_REQUIRE(server_future.get() == 0);
    CATCH_REQUIRE(client_future.get() == 0);
}
