#include "calebrjc/net/endpoint.hpp"

#include <arpa/inet.h>

#include <sstream>

#include "calebrjc/net/buffer.hpp"

namespace calebrjc::net {
namespace detail {
/// @brief Return the in_addr or in6_addr associated with the address contained in ss.
/// @param ss The address to be inspected.
/// @return The in_addr or in6_addr associated with the address contained in ss.
void *get_in_addr(sockaddr_storage ss) {
    if (ss.ss_family == AF_INET) { return &(((sockaddr_in *)&ss)->sin_addr); }

    return &(((sockaddr_in6 *)&ss)->sin6_addr);
}

/// @brief Return the port number associated with the address contained in ss.
/// @param ss The address to be inspected.
/// @return The port number associated with the address contained in ss.
uint16_t get_port(sockaddr_storage ss) {
    // Both sockaddr_in and sockaddr_in6 begin with (where "x" is "" or "6")
    //
    // struct sockaddr_inx {
    //     uint16_t sinx_family;
    //     uint16_t sinx_port
    // }
    //
    // Meaning that this access will return the same value for both types. Therefore, only one
    // implementation is used.
    return ntohs(((sockaddr_in *)&ss)->sin_port);
}
}  // namespace detail

Endpoint::Endpoint() {}

char *Endpoint::data() const {
    return (char *)&storage_;
}

std::string Endpoint::addr() const {
    Buffer buffer(INET6_ADDRSTRLEN);
    inet_ntop(AF_INET, detail::get_in_addr(storage_), buffer.data(), buffer.size());

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
            return std::to_string((int)detail::get_port(storage_));
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
