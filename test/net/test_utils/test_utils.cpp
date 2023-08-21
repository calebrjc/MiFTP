#include "test_utils.hpp"

#include "calebrjc/net/resolve.hpp"
#include "calebrjc/net/detail/socket_ops.hpp"

using namespace calebrjc;

void simulate_incoming_connection(
    const std::string &hostname, const std::string &service, sim_client_state &scs) {
    // Wait for the server to be ready for an incoming connection
    while (!scs.server_ready) {}

    // Connect to the server
    auto endpoints = net::resolve(hostname, service);
    socket_type socket_fd = net::detail::socket_ops::create_connected_socket(endpoints);
    scs.client_ready = true;

    // Wait for the server to accept the connection
    while (!scs.server_accepted_client) {}

    // Stay open until the server is done with the connection
    while (!scs.server_done) {}

    net::detail::socket_ops::close_socket(socket_fd);
}
