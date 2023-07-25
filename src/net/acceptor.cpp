#include "calebrjc/net/acceptor.hpp"

namespace calebrjc::net {
Acceptor::Acceptor() {}

Acceptor::~Acceptor() {}

void Acceptor::open(ResolveResult local_endpoints) {
    (void)local_endpoints;
}

void Acceptor::open(ResolveResult local_endpoints, std::error_code &ec) {
    (void)local_endpoints;
    (void)ec;
}

void Acceptor::close() {}

void Acceptor::close(std::error_code &ec) {
    (void)ec;
}

bool Acceptor::is_open() const {
    return false;
}

bool Acceptor::is_pending() const {
    return false;
}

Connection Acceptor::accept() const {
    return Connection();
}

Connection Acceptor::accept(std::error_code &ec) const {
    (void)ec;

    return Connection();
}

Endpoint Acceptor::local_endpoint() const {
    return Endpoint();
}
}  // namespace calebrjc::net
