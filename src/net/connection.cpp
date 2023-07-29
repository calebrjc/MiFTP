#include "calebrjc/net/connection.hpp"

#include <unistd.h>

#include "calebrjc/net/detail/addr_ops.hpp"

namespace calebrjc::net {
static const size_t max_buffer_size = 8192;

Connection::Connection() : socket_(0) {}

Connection Connection::from_fd(int socket_fd) {
    Connection conn;

    conn.socket_ = socket_fd;

    sockaddr_storage local_ss;
    socklen_t local_ss_len = sizeof(local_ss);
    ::getsockname(conn.socket_, (sockaddr *)&local_ss, &local_ss_len);
    std::memcpy(conn.local_endpoint_.data(), &local_ss, local_ss_len);

    sockaddr_storage remote_ss;
    socklen_t remote_ss_len = sizeof(local_ss);
    ::getpeername(conn.socket_, (sockaddr *)&remote_ss, &remote_ss_len);
    std::memcpy(conn.remote_endpoint_.data(), &remote_ss, remote_ss_len);

    return conn;
}

Connection::~Connection() {
    if (is_connected()) disconnect();
}

Connection::Connection(Connection &&other) {
    *this = std::move(other);
}

Connection &Connection::operator=(Connection &&other) {
    socket_ = other.socket_;
    local_endpoint_ = other.local_endpoint_;
    remote_endpoint_ = other.remote_endpoint_;

    other.socket_ = 0;
    other.local_endpoint_ = Endpoint();
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
        sockaddr_storage *ss = (sockaddr_storage *)endpoint.data();

        socket_fd = ::socket(ss->ss_family, SOCK_STREAM, 0);
        if (socket_fd == -1) { continue; }

        int connect_result =
            ::connect(socket_fd, (sockaddr *)ss, sizeof(*ss));
        if (connect_result == -1) { continue; }

        socket_ = socket_fd;
        remote_endpoint_ = endpoint;
        break;
    }

    if (socket_ == 0) {
        ec.assign(errno, std::system_category());
        return;
    }

    sockaddr_storage local_ss;
    socklen_t local_ss_len = sizeof(local_ss);
    ::getsockname(socket_, (sockaddr *)&local_ss, &local_ss_len);
    std::memcpy(local_endpoint_.data(), &local_ss, local_ss_len);
}

void Connection::disconnect() {
    ::shutdown(socket_, SHUT_RDWR);
    ::close(socket_);

    socket_          = 0;
    local_endpoint_  = Endpoint();
    remote_endpoint_ = Endpoint();
}

void Connection::send(Buffer data) const {
    // Delegate function call and throw if necessary
    std::error_code ec;
    send(data, ec);

    if (ec) throw ec;
}

void Connection::send(Buffer data, std::error_code &ec) const {
    char *send_buffer       = data.data();
    size_t send_buffer_size = data.size();

    int bytes_sent = 0;
    while (bytes_sent < data.size()) {
        int send_result =
            ::send(socket_, send_buffer + bytes_sent, send_buffer_size - bytes_sent, 0);
        if (send_result == -1) {
            ec.assign(errno, std::system_category());
            return;
        }
        bytes_sent += send_result;
    }
}

Buffer Connection::receive() const {
    // Delegate function call and throw if necessary
    std::error_code ec;
    auto data = receive(ec);

    if (ec) throw ec;

    return data;
}

Buffer Connection::receive(std::error_code &ec) const {
    char receive_buffer[max_buffer_size];

    int recv_result = ::recv(socket_, receive_buffer, max_buffer_size, 0);
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
