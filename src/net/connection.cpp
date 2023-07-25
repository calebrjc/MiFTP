#include "calebrjc/net/connection.hpp"

namespace calebrjc::net {
Connection::Connection() {}

Connection::Connection(ResolveResult local_endpoint) {
    (void)local_endpoint;
}

Connection::~Connection() {}

Connection::Connection(const Connection &&other) {
    (void)other;
}

Connection &Connection::operator=(const Connection &&other) {
    (void)other;
    return *this;
}

void Connection::bind(ResolveResult local_endpoint) {
    (void)local_endpoint;
}

void Connection::bind(ResolveResult local_endpoint, std::error_code &ec) {
    (void)local_endpoint;
}

void Connection::connect(ResolveResult remote_endpoint) {
    (void)remote_endpoint;
}

void Connection::connect(ResolveResult remote_endpoint, std::error_code &ec) {
    (void)remote_endpoint;
    (void)ec;
}

void Connection::disconnect() {}

void Connection::disconnect(std::error_code &ec) {}

void Connection::send(Buffer data) const {
    (void)data;
}

void Connection::send(Buffer data, std::error_code &ec) const {
    (void)data;
    (void)ec;
}

Buffer Connection::receive() const {
    return Buffer();
}

Buffer Connection::receive(std::error_code &ec) const {
    (void)ec;
    return Buffer();
}

bool Connection::is_open() const {
    return false;
}

Endpoint Connection::local_endpoint() const {
    return Endpoint();
}
Endpoint Connection::remote_endpoint() const {
    return Endpoint();
}
}  // namespace calebrjc::net
