#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#include "calebrjc/net/net.hpp"

#define PORT    "3490"  // the port users will be connecting to
#define BACKLOG 10      // how many pending connections queue will hold

using namespace calebrjc;

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) { return &(((struct sockaddr_in *)sa)->sin_addr); }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

void c_style() {
    // Get remote address info
    addrinfo hints;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE;

    addrinfo *server_info;
    if (int result = getaddrinfo(NULL, PORT, &hints, &server_info) != 0) {
        std::cout << "getaddrinfo() failed: " << gai_strerror(result) << "\n";
        std::exit(EXIT_FAILURE);
    }

    int socket_fd;
    addrinfo *p = server_info;
    for (; p != NULL; p = p->ai_next) {
        // Try to connect to the first entry we can
        if ((socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
            std::perror("socket() failed");
            continue;
        }

        int yes = 1;
        if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0) {
            std::perror("setsockopt() failed");
            exit(EXIT_FAILURE);
        }

        if (bind(socket_fd, p->ai_addr, p->ai_addrlen) < 0) {
            close(socket_fd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(server_info);

    if (p == NULL) {
        std::cerr << "server: failed to bind\n";
        exit(EXIT_FAILURE);
    }

    if (listen(socket_fd, BACKLOG) < 0) {
        perror("listen() failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        std::cout << "server: waiting for connection...\n";

        sockaddr_storage remote_addr;
        socklen_t remote_addr_size = sizeof(remote_addr);
        int remote_fd              = 0;
        if ((remote_fd = accept(socket_fd, (struct sockaddr *)&remote_addr, &remote_addr_size))
            < 0) {
            perror("accept() failed");
            exit(EXIT_FAILURE);
        }

        net::Buffer remote_addr_str(INET6_ADDRSTRLEN);
        inet_ntop(
            remote_addr.ss_family,
            get_in_addr((struct sockaddr *)&remote_addr),
            remote_addr_str.data(),
            remote_addr_str.size());
        printf("server: got connection from %s\n", remote_addr_str.data());

        net::Buffer recv_buffer(1024);
        if (recv(remote_fd, recv_buffer.data(), recv_buffer.size(), 0) < 0) {
            perror("recv() failed");
            exit(EXIT_FAILURE);
        }
        std::cout << "Data received: " << std::string(recv_buffer.data(), recv_buffer.size());

        net::Buffer send_buffer(recv_buffer.data(), strnlen(recv_buffer.data(), 1024));
        int bytes_sent = 0;
        if ((bytes_sent = send(remote_fd, send_buffer.data(), send_buffer.size(), 0)) < 0) {
            perror("send() failed");
            exit(EXIT_FAILURE);
        }
        std::cout << "Sent " << bytes_sent << " bytes\n";

        shutdown(remote_fd, SHUT_RDWR);
        close(remote_fd);
    }
    close(socket_fd);
}

void new_style_throw() {
    // Open the acceptor
    calebrjc::net::Acceptor acceptor;
    auto server_endpoints = net::resolve(net::all_local_addresses, PORT);
    acceptor.open(server_endpoints);

    while (1) {
        // Wait for an incoming connection
        std::cout << "[server] waiting for connection at " << acceptor.local_endpoint().str()
                  << "...\n";
        auto client_conn = acceptor.accept();

        // Receive a message
        net::Buffer data = client_conn.receive();
        std::cout << "[server] message received: " << data.str() << "\n";

        // Echo the message
        client_conn.send(data);
        std::cout << "[server] sent " << data.size() << " bytes to "
                  << client_conn.remote_endpoint().str() << "\n";
    }
}

void new_style_error_code() {
    net::Acceptor acceptor;
    std::error_code ec;

    // Resolve the server's address
    auto server_endpoints = net::resolve(net::all_local_addresses, PORT, ec);
    if (ec) { std::cout << "[server] unable to resolve local address\n"; }

    std::cout << "[server] resolved endpoints:\n";
    for (auto &e : server_endpoints) { std::cout << "[server]\t" << e.str() << "\n"; }
}

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    c_style();
}
