#include "calebrjc/net/connection.hpp"

#include <unistd.h>

#include "calebrjc/net/detail/sockaddr_ops.hpp"
#include "calebrjc/net/detail/socket_ops.hpp"

static const size_t max_buffer_size = 8192;

namespace calebrjc::net {
Connection::Connection() : socket_(0) {}

Connection Connection::from_native_socket(int socket_fd, Endpoint remote_endpoint) {
    Connection conn;
    conn.socket_          = socket_fd;
    conn.local_endpoint_  = detail::socket_ops::get_local_endpoint(conn.socket_);
    conn.remote_endpoint_ = remote_endpoint;

    return conn;
}

Connection::~Connection() {
    if (is_connected()) disconnect();
}

Connection::Connection(Connection &&other) {
    *this = std::move(other);
}

Connection &Connection::operator=(Connection &&other) {
    socket_          = other.socket_;
    local_endpoint_  = other.local_endpoint_;
    remote_endpoint_ = other.remote_endpoint_;

    other.socket_          = 0;
    other.local_endpoint_  = Endpoint();
    other.remote_endpoint_ = Endpoint();

    return *this;
}

void Connection::connect(ResolveResult remote) {
    // Delegate function call and throw if necessary
    std::error_code ec;
    connect(remote, ec);

    if (ec) throw ec;
}

void Connection::connect(ResolveResult remote, std::error_code &ec) {
    // Attempt to create a connected socket
    int socket_fd = 0;
    for (auto &endpoint : remote) {
        socket_fd = ::socket(endpoint.family(), SOCK_STREAM, endpoint.protocol());
        if (socket_fd == -1) { continue; }

        int connect_result = ::connect(socket_fd, endpoint.data(), endpoint.size());
        if (connect_result == -1) { continue; }

        socket_          = socket_fd;
        remote_endpoint_ = endpoint;
        local_endpoint_  = detail::socket_ops::get_local_endpoint(socket_);
        break;
    }

    if (socket_ == 0) {
        // TODO(Caleb): Better error handling
        ec.assign(errno, std::system_category());
        return;
    }
}

void Connection::disconnect() {
    ::shutdown(socket_, SHUT_RDWR);
    ::close(socket_);

    socket_          = 0;
    local_endpoint_  = Endpoint();
    remote_endpoint_ = Endpoint();
}

void Connection::send(const Buffer &data, send_flags_mask flags) const {
    // Delegate function call and throw if necessary
    std::error_code ec;
    send(data, flags, ec);

    if (ec) throw ec;
}

void Connection::send(const Buffer &data, std::error_code &ec) const {
    send(data, send_flags::none, ec);
}

void Connection::send(const Buffer &data, send_flags_mask flags, std::error_code &ec) const {
    const char *send_buffer = data.data();
    size_t send_buffer_size = data.size();

    // Translate flags
    int send_flags = 0;
    send_flags |= (flags & send_flags::dont_route) ? MSG_DONTROUTE : 0;
    send_flags |= (flags & send_flags::end_of_record) ? MSG_EOR : 0;

    int bytes_sent = 0;
    while (bytes_sent < data.size()) {
        int send_result =
            ::send(socket_, send_buffer + bytes_sent, send_buffer_size - bytes_sent, send_flags);
        if (send_result == -1) {
            ec.assign(errno, std::system_category());
            return;
        }
        bytes_sent += send_result;
    }
}

Buffer Connection::receive(receive_flags_mask flags) const {
    // Delegate function call and throw if necessary
    std::error_code ec;
    auto data = receive(flags, ec);

    if (ec) throw ec;

    return data;
}

Buffer Connection::receive(std::error_code &ec) const {
    return receive(receive_flags::none, ec);
}

Buffer Connection::receive(receive_flags_mask flags, std::error_code &ec) const {
    char receive_buffer[max_buffer_size];

    // Translate flags
    int recv_flags = 0;
    recv_flags |= (flags & receive_flags::peek) ? MSG_PEEK : 0;

    int recv_result = ::recv(socket_, receive_buffer, max_buffer_size, recv_flags);
    if (recv_result == -1) {
        ec.assign(errno, std::system_category());
        return Buffer();
    }

    return Buffer(receive_buffer, recv_result);
}

bool Connection::is_connected() const {
    return socket_ == 0;
}

Endpoint Connection::local_endpoint() const {
    return local_endpoint_;
}
Endpoint Connection::remote_endpoint() const {
    return remote_endpoint_;
}
}  // namespace calebrjc::net
