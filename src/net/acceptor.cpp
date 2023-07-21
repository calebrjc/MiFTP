#include "calebrjc/net/acceptor.hpp"

namespace calebrjc::net {
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
}
