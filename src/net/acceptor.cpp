#include "calebrjc/net/acceptor.hpp"

#include <sys/socket.h>
#include <unistd.h>

#include "calebrjc/net/detail/socket_ops.hpp"

namespace calebrjc::net {
/// @brief The maximum number of connections that an acceptor may queue for accepting.
static const int backlog_size = 128;

acceptor::acceptor() : socket_(0) {}

acceptor::~acceptor() {
    if (is_open()) close();
}

void acceptor::open(resolve_result local_endpoints, bool reuse_addr) {
    // Delegate function call and throw if necessary
    std::error_code ec;
    open(local_endpoints, ec, reuse_addr);

    if (ec) throw ec;
}

void acceptor::open(resolve_result local_endpoints, std::error_code &ec, bool reuse_addr) {
    int socket_fd = 0;
    for (auto &endpoint : local_endpoints) {
        // Attempt to get a socket handle
        socket_fd = ::socket(endpoint.family(), SOCK_STREAM, endpoint.protocol());
        if (socket_fd == -1) continue;

        // Enable SO_REUSEADDR if necessary
        if (reuse_addr) {
            int on         = 1;
            int sso_result = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
            if (sso_result == -1) continue;
        }

        int bind_result = ::bind(socket_fd, endpoint.data(), endpoint.size());
        if (bind_result == -1) continue;

        socket_         = socket_fd;
        local_endpoint_ = endpoint;
    }

    int listen_result = ::listen(socket_fd, backlog_size);
    if (listen_result == -1) {
        ec.assign(errno, std::system_category());
        return;
    }
}

void acceptor::close() {
    ::shutdown(socket_, SHUT_RDWR);
    ::close(socket_);

    socket_         = 0;
    local_endpoint_ = endpoint();
}

bool acceptor::is_open() const {
    return socket_ == 0;
}

bool acceptor::is_pending() const {
    return detail::socket_ops::is_ready_to_read(socket_, 0);
    // return detail::socket_ops::poll_socket(socket_, detail::socket_status::readable)
}

connection acceptor::accept() const {
    // Delegate function call and throw if necessary
    std::error_code ec;
    auto conn = accept(ec);

    if (ec) throw ec;

    return conn;
}

connection acceptor::accept(std::error_code &ec) const {
    sockaddr_storage remote_addr;
    socklen_t remote_addr_size = sizeof(remote_addr);

    int remote_socket_fd = ::accept(socket_, (sockaddr *)&remote_addr, &remote_addr_size);
    if (remote_socket_fd == -1) {
        ec.assign(errno, std::system_category());
        return connection();
    }

    auto remote_endpoint = endpoint::from_native_address(
        local_endpoint_.protocol(), (sockaddr *)&remote_addr, remote_addr_size);
    return connection::from_native_socket(remote_socket_fd, remote_endpoint);
}

endpoint acceptor::local_endpoint() const {
    return local_endpoint_;
}
}  // namespace calebrjc::net
