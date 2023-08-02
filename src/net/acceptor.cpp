#include "calebrjc/net/acceptor.hpp"

#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>

namespace calebrjc::net {
static const int backlog_size = 128;

// TODO(Caleb): New file/namespace for socket operations?
namespace detail {
bool is_ready_to_read(int socket_fd, int timeout_millis) {
    pollfd pollfd;
    pollfd.fd = socket_fd;
    pollfd.events |= POLLIN;

    int events_fired = ::poll(&pollfd, 1, timeout_millis);
    return (events_fired > 0);
}
}  // namespace detail

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
        //? Note(Caleb): Is there a better way to do this cast?
        sockaddr_storage *ss = (sockaddr_storage *)endpoint.data();

        socket_fd = ::socket(ss->ss_family, SOCK_STREAM, 0);
        if (socket_fd == -1) continue;

        int on        = 1;
        int sso_result = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
        if (sso_result == -1) continue;

        int bind_result = ::bind(socket_fd, (sockaddr *)ss, sizeof(*ss));
        if (bind_result == -1) continue;

        socket_ = socket_fd;

        // TODO(Caleb): DRY this up (see: connection.cpp)
        sockaddr_storage local_ss;
        socklen_t local_ss_len = sizeof(local_ss);
        ::getsockname(socket_, (sockaddr *)&local_ss, &local_ss_len);
        std::memcpy(local_endpoint_.data(), &local_ss, local_ss_len);
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
    return detail::is_ready_to_read(socket_, 0);
}

Connection Acceptor::accept() const {
    // Delegate function call and throw if necessary
    std::error_code ec;
    auto conn = accept(ec);

    if (ec) throw ec;
    
    return conn;
}

Connection Acceptor::accept(std::error_code &ec) const {
    // TODO(Caleb): Remove these unused variables
    sockaddr_storage remote_addr;
    socklen_t remote_addr_size = sizeof(remote_addr);

    int remote_socket_fd = ::accept(socket_, (sockaddr *)&remote_addr, &remote_addr_size);
    if (remote_socket_fd == -1) {
        ec.assign(errno, std::system_category());
        return Connection();
    }

    return Connection::from_fd(remote_socket_fd);
}

Endpoint Acceptor::local_endpoint() const {
    return local_endpoint_;
}
}  // namespace calebrjc::net
