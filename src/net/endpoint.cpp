#include "calebrjc/net/endpoint.hpp"

#include <arpa/inet.h>

#include <sstream>

#include "calebrjc/net/buffer.hpp"
#include "calebrjc/net/detail/addr_ops.hpp"

namespace calebrjc::net {
Endpoint::Endpoint() {}

char *Endpoint::data() const {
    return (char *)&storage_;
}

std::string Endpoint::addr() const {
    Buffer buffer(INET6_ADDRSTRLEN);
    inet_ntop(AF_INET, detail::addr_ops::get_in_addr(&storage_), buffer.data(), buffer.size());

    switch (storage_.ss_family) {
        case AF_INET:
            return std::string(buffer.data(), INET_ADDRSTRLEN);
        case AF_INET6:
            return std::string(buffer.data(), INET6_ADDRSTRLEN);
        default:
            return "INVALID";
    }
}

std::string Endpoint::port() const {
    switch (storage_.ss_family) {
        case AF_INET:
        case AF_INET6:
            return std::to_string((int)detail::addr_ops::get_port(&storage_));
        default:
            return "INVALID";
    }
}

std::string Endpoint::str() const {
    std::stringstream ss;
    switch (storage_.ss_family) {
        case AF_INET:
            ss << addr() << ":" << port();
            return ss.str();
        case AF_INET6:
            ss << "[" << addr() << "]:" << port();
            return ss.str();
        default:
            return "INVALID:INVALID";
    }
}
}  // namespace calebrjc::net
