#include "calebrjc/net/net.hpp"

#include <iostream>

using namespace calebrjc;

int main(int argc, char **argv) {
    if (argc != 2) return EXIT_FAILURE;

    std::string service(argv[1]);

    net::acceptor acceptor;
    std::error_code ec;

    // Resolve the server's address
    auto server_endpoints = net::resolve(net::any_address, service, ec);
    if (ec) return 64;

    acceptor.open(server_endpoints, ec, true);
    if (ec) return 65;

    auto client_conn = acceptor.accept(ec);
    if (ec) return 66;

    net::buffer data = client_conn.receive(ec);
    if (ec) return 67;

    client_conn.send(data);
    if (ec) return 68;
}
