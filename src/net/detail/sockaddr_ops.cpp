#include "calebrjc/net/detail/sockaddr_ops.hpp"

namespace calebrjc::net::detail {
namespace sockaddr_ops {
void *get_in_addr(const sockaddr *sa) {
    if (sa->sa_family == AF_INET) { return &(((sockaddr_in *)sa)->sin_addr); }

    return &(((sockaddr_in6 *)sa)->sin6_addr);
}

uint16_t get_port(const sockaddr *sa) {
    // Both sockaddr_in and sockaddr_in6 begin with (where "x" is "" or "6")
    //
    // struct sockaddr_inx {
    //     uint16_t sinx_family;
    //     uint16_t sinx_port
    // }
    //
    // Meaning that this access will return the same value for both types. Therefore, only one
    // implementation is used.
    return ntohs(((sockaddr_in *)sa)->sin_port);
}
}  // namespace sockaddr_ops
}  // namespace calebrjc::net::detail