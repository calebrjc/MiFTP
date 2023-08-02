#include "calebrjc/net/net.hpp"

using namespace calebrjc;

int main(int argc, char **argv) {
    if (argc != 2) return EXIT_FAILURE;

    std::string service(argv[1]);

    net::Acceptor acceptor;
    std::error_code ec;

    // Resolve the server's address
    auto server_endpoints = net::resolve(net::all_local_interfaces, service, ec);
    if (ec) return EXIT_FAILURE;

    acceptor.open(server_endpoints, ec);
    if (ec) return EXIT_FAILURE;

    auto client_conn = acceptor.accept(ec);
    if (ec) return EXIT_FAILURE;

    net::Buffer data = client_conn.receive(ec);
    if (ec) return EXIT_FAILURE;

    client_conn.send(data);
    if (ec) return EXIT_FAILURE;
}
