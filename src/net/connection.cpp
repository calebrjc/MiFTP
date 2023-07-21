#include "calebrjc/net/connection.hpp"

namespace calebrjc::net {
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
}
