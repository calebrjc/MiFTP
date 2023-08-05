#include "calebrjc/net/acceptor.hpp"

#include <sys/socket.h>
#include <unistd.h>

#include "calebrjc/net/detail/socket_ops.hpp"

namespace calebrjc::net {
static const int backlog_size = 128;

Acceptor::Acceptor() : socket_(0) {}

Acceptor::~Acceptor() {
    if (is_open()) close();
}

void Acceptor::open(ResolveResult local_endpoint) {
    // Delegate function call and throw if necessary
    std::error_code ec;
    open(local_endpoint, ec);

    if (ec) throw ec;
}

void Acceptor::open(ResolveResult local_endpoint, std::error_code &ec) {
    int socket_fd = 0;
    for (auto &endpoint : local_endpoint) {
        // Attempt to get a socket handle
        socket_fd = ::socket(endpoint.family(), SOCK_STREAM, endpoint.protocol());
        if (socket_fd == -1) continue;

        // Enable SO_REUSEADDR
        //? Note(Caleb): Should SO_REUSEADDR be an option available in the API?
        int on         = 1;
        int sso_result = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
        if (sso_result == -1) continue;

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

void Acceptor::close() {
    ::shutdown(socket_, SHUT_RDWR);
    ::close(socket_);

    socket_         = 0;
    local_endpoint_ = Endpoint();
}

bool Acceptor::is_open() const {
    return socket_ == 0;
}

bool Acceptor::is_pending() const {
    return detail::socket_ops::is_ready_to_read(socket_, 0);
}

Connection Acceptor::accept() const {
    // Delegate function call and throw if necessary
    std::error_code ec;
    auto conn = accept(ec);

    if (ec) throw ec;

    return conn;
}

Connection Acceptor::accept(std::error_code &ec) const {
    sockaddr_storage remote_addr;
    socklen_t remote_addr_size = sizeof(remote_addr);
    
    int remote_socket_fd = ::accept(socket_, (sockaddr *)&remote_addr, &remote_addr_size);
    if (remote_socket_fd == -1) {
        ec.assign(errno, std::system_category());
        return Connection();
    }

    auto remote_endpoint = Endpoint::from_native_address(
        local_endpoint_.protocol(), (sockaddr *)&remote_addr, remote_addr_size);
    return Connection::from_fd(remote_socket_fd, remote_endpoint);
}

Endpoint Acceptor::local_endpoint() const {
    return local_endpoint_;
}
}  // namespace calebrjc::net
