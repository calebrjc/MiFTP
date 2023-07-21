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

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) { return &(((struct sockaddr_in *)sa)->sin_addr); }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;
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

        calebrjc::Buffer remote_addr_str(INET6_ADDRSTRLEN);
        inet_ntop(
            remote_addr.ss_family,
            get_in_addr((struct sockaddr *)&remote_addr),
            remote_addr_str.data(),
            remote_addr_str.size());
        printf("server: got connection from %s\n", remote_addr_str.data());

        calebrjc::Buffer recv_buffer(1024);
        if (recv(remote_fd, recv_buffer.data(), recv_buffer.size(), 0) < 0) {
            perror("recv() failed");
            exit(EXIT_FAILURE);
        }
        std::cout << "Data received: " << std::string(recv_buffer.data(), recv_buffer.size());

        calebrjc::Buffer send_buffer(recv_buffer.data(), strnlen(recv_buffer.data(), 1024));
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

void new_style() {
    // Open the acceptor
    calebrjc::net::Acceptor acceptor({PORT});

    // Make sure the acceptor is opern
    if (!acceptor.is_open()) {
        std::cout << "[server] error: failed to open acceptor\n";
        exit(EXIT_FAILURE);
    }

    while (1) {
        std::cout << "[server] waiting for connection...\n";

        if (!acceptor.is_pending()) acceptor.wait();
        calebrjc::net::Connection client_conn = acceptor.accept();

        calebrjc::Buffer data = client_conn.recv();
        if (!data) {
            std::cout << "[server] error: failed to receive message\n";
            exit(EXIT_FAILURE);
        }
        std::cout << "[server] message received: " << data.str() << "\n";

        bool sent = client_conn.send(data);
        if (!sent) {
            std::cout << "[server] error: failed to receive message\n";
            exit(EXIT_FAILURE);
        }
        std::cout << "[server] sent " << data.size() << " bytes to "
                  << client_conn.remote_endpoint().str() << "\n";
    }
}
