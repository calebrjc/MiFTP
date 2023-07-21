#include "calebrjc/net/endpoint.hpp"

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
}
