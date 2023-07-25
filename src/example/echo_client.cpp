#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "calebrjc/net/net.hpp"

#define ADDRESS         "localhost"
#define PORT            "3490"
#define MESSAGE         "Hello!\n"
#define MAX_BUFFER_SIZE 1024

using namespace calebrjc;

void perror_and_exit(const char *header) {
    std::perror(header);
    std::exit(EXIT_FAILURE);
}

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) { return &(((struct sockaddr_in *)sa)->sin_addr); }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

void c_style() {
    // Prepare hints
    addrinfo hints;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    // Address resolution
    addrinfo *server_info;
    int result = 0;
    if ((result = getaddrinfo(ADDRESS, PORT, &hints, &server_info)) != 0) {
        std::cout << "getaddrinfo() failed: " << gai_strerror(result) << "\n";
        std::exit(EXIT_FAILURE);
    }

    // Try connecting to any of the resolved addresses
    int socket_fd;
    addrinfo *p;
    for (p = server_info; p != NULL; p = p->ai_next) {
        // Try to connect to the first entry we can
        if ((socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
            std::perror("socket() failed");
            continue;
        }

        if (connect(socket_fd, p->ai_addr, p->ai_addrlen) < 0) {
            std::perror("connect() failed");
            continue;
        }

        break;
    }

    // Fail if we can't
    if (p == NULL) {
        std::cerr << "miftp: Failed to connect to server\n";
        exit(EXIT_FAILURE);
    }

    // Who did we connect to?
    net::Buffer addr_str_buf(INET6_ADDRSTRLEN);
    inet_ntop(
        p->ai_family,
        get_in_addr((sockaddr *)p->ai_addr),
        addr_str_buf.data(),
        addr_str_buf.size());
    printf("client: connecting to %s\n", addr_str_buf.data());

    freeaddrinfo(server_info);

    // Send message
    net::Buffer send_buffer(MESSAGE);
    result = send(socket_fd, send_buffer.data(), send_buffer.size(), 0);
    if (result < 0) perror_and_exit("send() failed");
    std::cout << "Sent " << result << " bytes\n";

    // Receive echo
    net::Buffer recv_buffer(MAX_BUFFER_SIZE);
    recv_buffer.zero();
    result = recv(socket_fd, recv_buffer.data(), recv_buffer.size(), 0);
    if (result < 0) perror_and_exit("recv() failed");
    std::cout << "Data received: " << std::string(recv_buffer.data(), recv_buffer.size());

    // Cleanup
    shutdown(socket_fd, SHUT_RDWR);
    close(socket_fd);
}

void new_style_throw() {
    // Open the connection
    net::Connection conn;
    auto server_endpoints = net::resolve(ADDRESS, PORT);
    conn.connect(server_endpoints);
    std::cout << "[client] connected to " << conn.remote_endpoint().str() << "\n";

    // Send message
    conn.send(net::Buffer(MESSAGE));
    std::cout << "[client] message sent\n";

    // Receive message
    net::Buffer response = conn.receive();
    std::cout << "[client] message received: " << response.str() << "\n";
}

void new_style_error_code() {
    net::Connection conn;
    std::error_code ec;

    // Resolve the server's address
    auto server_endpoints = net::resolve(ADDRESS, PORT, ec);
    if (ec) { std::cout << "[client] unable to resolve address\n"; }

    // Open the connection
    conn.connect(server_endpoints, ec);
    if (ec) {
        std::cout << "[client] error: failed to esablish connection\n";
        exit(EXIT_FAILURE);
    }
    std::cout << "[client] connected to " << conn.remote_endpoint().str() << "\n";

    // Send message
    conn.send(net::Buffer(MESSAGE), ec);
    if (ec) {
        std::cout << "[client] error: failed to send message\n";
        exit(EXIT_FAILURE);
    }
    std::cout << "[client] message sent\n";

    // Receive message
    net::Buffer response = conn.receive(ec);
    if (ec) {
        std::cout << "[client] error: failed to receive message\n";
        exit(EXIT_FAILURE);
    }
    std::cout << "[client] message received: " << response.str() << "\n";
}

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    c_style();
}
