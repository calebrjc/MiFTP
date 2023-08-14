#include "calebrjc/net/endpoint.hpp"

#include <arpa/inet.h>

#include <sstream>

#include "calebrjc/net/buffer.hpp"
#include "calebrjc/net/detail/sockaddr_ops.hpp"

namespace calebrjc::net {
endpoint::endpoint() {}

endpoint endpoint::from_native_address(int protocol, sockaddr *addr, socklen_t addr_size) {
    endpoint e;
    e.protocol_ = protocol;
    std::memcpy(e.data(), addr, addr_size);
    e.storage_size_ = addr_size;

    return e;
}

sa_family_t endpoint::family() const {
    return storage_.ss_family;
}

int endpoint::protocol() const {
    return protocol_;
}

sockaddr *endpoint::data() {
    //? Note(Caleb): Maybe use a buffer and union type?
    return (sockaddr *)&storage_;
}

const sockaddr *endpoint::data() const {
    //? Note(Caleb): Maybe use a buffer and union type?
    return (sockaddr *)&storage_;
}

socklen_t endpoint::size() const {
    return storage_size_;
}

std::string endpoint::addr() const {
    buffer buffer(INET6_ADDRSTRLEN);
    inet_ntop(AF_INET, detail::sockaddr_ops::get_in_addr(data()), buffer.data(), buffer.size());

    switch (storage_.ss_family) {
        case AF_INET:
            return std::string(buffer.data(), INET_ADDRSTRLEN);
        case AF_INET6:
            return std::string(buffer.data(), INET6_ADDRSTRLEN);
        default:
            return "INVALID";
    }
}

std::string endpoint::port() const {
    switch (storage_.ss_family) {
        case AF_INET:
        case AF_INET6:
            return std::to_string((int)detail::sockaddr_ops::get_port(data()));
        default:
            return "INVALID";
    }
}

std::string endpoint::str() const {
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
