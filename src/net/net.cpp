#include "net/net.hpp"

namespace calebrjc::net {
Endpoint::Endpoint() {}

Endpoint::Endpoint(std::string hostname, std::string service) {
    (void)hostname;
    (void)service;
}

Endpoint::Endpoint(std::string hostname, uint16_t port_number)
    : Endpoint(hostname, std::to_string(port_number)) {}

Endpoint::Endpoint(std::string service) : Endpoint("localhost", service) {}

Endpoint::Endpoint(uint16_t port_number) : Endpoint("localhost", port_number) {}

std::string Endpoint::addr() const {
    return "";
}

std::string Endpoint::port() const {
    return "";
}

std::string Endpoint::str() const {
    return "";
}

Connection::Connection() {}

Connection::Connection(Endpoint remote_endpoint) {
    (void)remote_endpoint;
}

Connection::~Connection() {}

void Connection::open(Endpoint remote_endpoint) {
    (void)remote_endpoint;
}

void Connection::close() {}

bool Connection::send(Buffer data) {
    (void)data;
    return false;
}

Buffer Connection::recv() {
    return Buffer();
}

bool Connection::is_open() {
    return false;
}

Endpoint Connection::local_endpoint() {
    return Endpoint();
}
Endpoint Connection::remote_endpoint() {
    return Endpoint();
}

Acceptor::Acceptor() {}

Acceptor::Acceptor(Endpoint local_endpoint) {
    (void)local_endpoint;
}

Acceptor::~Acceptor() {}

void Acceptor::open(Endpoint local_endpoint) {
    (void)local_endpoint;
}

void Acceptor::close() {}

bool Acceptor::is_open() {
    return false;
}

bool Acceptor::is_pending() {
    return false;
}

Connection Acceptor::accept() {
    return Connection();
}

void Acceptor::wait() {}
}  // namespace calebrjc::net
