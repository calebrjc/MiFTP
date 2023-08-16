#include "calebrjc/net/acceptor.hpp"

#include <sys/socket.h>

#include "calebrjc/net/detail/poll.hpp"
#include "calebrjc/net/detail/socket_ops.hpp"

namespace calebrjc::net {

/// @brief The maximum number of connections that an acceptor may queue for accepting.
static const int backlog_size = 128;

acceptor::acceptor() : socket_(0) {}

acceptor::~acceptor() {
    if (is_open()) close();
}

void acceptor::open(const resolve_result &local_endpoints, acceptor_config_mask cfg) {
    // Delegate function call and throw if necessary
    std::error_code ec;
    open(local_endpoints, ec, cfg);

    if (ec) throw ec;
}

void acceptor::open(
    const resolve_result &local_endpoints, std::error_code &ec, acceptor_config_mask cfg) {
    if (local_endpoints.empty()) {
        // TODO(Caleb): Custom error categories?
        ec.assign(1, std::system_category());
        return;
    }

    int socket_fd = 0;
    for (const endpoint &e : local_endpoints) {
        // Attempt to get a socket handle
        socket_fd = ::socket(e.family(), SOCK_STREAM, e.protocol());
        if (socket_fd == -1) continue;

        // Enable SO_REUSEADDR if necessary
        if (cfg & acceptor_config::reuse_address) {
            int on         = 1;
            int sso_result = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
            if (sso_result == -1) continue;
        }

        int bind_result = ::bind(socket_fd, e.data(), e.size());
        if (bind_result == -1) continue;

        socket_         = socket_fd;
        local_endpoint_ = e;
    }

    if (socket_ == 0) {
        // TODO(Caleb): Custom error categories?
        ec.assign(errno, std::system_category());
    }

    int listen_result = ::listen(socket_fd, backlog_size);
    if (listen_result == -1) {
        // TODO(Caleb): Custom error categories?
        ec.assign(errno, std::system_category());
        return;
    }
}

void acceptor::close() {
    detail::socket_ops::close_socket(socket_);

    socket_         = 0;
    local_endpoint_ = endpoint();
}

bool acceptor::is_open() const {
    // Note(Caleb): socket_ is only assigned after a connect(), so this is fine.
    return socket_ == 0;
}

bool acceptor::has_pending_connection() const {
    detail::poll::socket_status_mask socket_status = detail::poll::poll_socket(socket_, 0);
    return (socket_status & detail::poll::socket_status::readable) != 0;
}

connection acceptor::accept() const {
    // Delegate function call and throw if necessary
    std::error_code ec;
    auto conn = accept(ec);

    if (ec) throw ec;

    return conn;
}

connection acceptor::accept(std::error_code &ec) const {
    address_storage_type remote_addr;
    address_size_type remote_addr_size = sizeof(remote_addr);

    int remote_socket_fd = ::accept(socket_, (address_type *)&remote_addr, &remote_addr_size);
    if (remote_socket_fd == -1) {
        // TODO(Caleb): Custom error categories?
        ec.assign(errno, std::system_category());
        return connection();
    }

    auto remote_endpoint = endpoint::from_native_address(
        local_endpoint_.protocol(), (address_type *)&remote_addr, remote_addr_size);
    return connection::from_native_socket(remote_socket_fd, remote_endpoint);
}

endpoint acceptor::local_endpoint() const {
    return local_endpoint_;
}

}  // namespace calebrjc::net
