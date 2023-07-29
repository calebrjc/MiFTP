#include "calebrjc/net/detail/addr_ops.hpp"

namespace calebrjc::net::detail {
namespace addr_ops {
void *get_in_addr(const sockaddr_storage *ss) {
    if (ss->ss_family == AF_INET) { return &(((sockaddr_in *)ss)->sin_addr); }

    return &(((sockaddr_in6 *)ss)->sin6_addr);
}

uint16_t get_port(const sockaddr_storage *ss) {
    // Both sockaddr_in and sockaddr_in6 begin with (where "x" is "" or "6")
    //
    // struct sockaddr_inx {
    //     uint16_t sinx_family;
    //     uint16_t sinx_port
    // }
    //
    // Meaning that this access will return the same value for both types. Therefore, only one
    // implementation is used.
    return ntohs(((sockaddr_in *)ss)->sin_port);
}
}  // namespace addr_ops
}  // namespace calebrjc::net::detail
