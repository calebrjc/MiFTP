#include "calebrjc/net/net.hpp"

#define MESSAGE "Hello!\n"

using namespace calebrjc;

int main(int argc, char **argv) {
    if (argc < 3 || 4 < argc) return EXIT_FAILURE;

    std::string name(argv[1]);
    std::string service(argv[2]);
    net::Buffer message(MESSAGE);

    net::Connection conn;
    std::error_code ec;

    // Resolve the server's address
    auto server_endpoints = net::resolve(name, service, ec);
    if (ec) return EXIT_FAILURE;

    // Open the connection
    conn.connect(server_endpoints, ec);
    if (ec) return EXIT_FAILURE;

    // Send message
    conn.send(message, ec);
    if (ec) return EXIT_FAILURE;

    // Receive message
    net::Buffer response = conn.receive(ec);
    if (ec) return EXIT_FAILURE;

    if (response != message) return EXIT_FAILURE;
}
