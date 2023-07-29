#pragma once

#include <arpa/inet.h>

namespace calebrjc::net::detail {
namespace addr_ops {
/// @brief Return the in_addr or in6_addr associated with the address contained in ss.
/// @param ss The address to be inspected.
/// @return The in_addr or in6_addr associated with the address contained in ss.
void *get_in_addr(const sockaddr_storage *ss);

/// @brief Return the port number associated with the address contained in ss.
/// @param ss The address to be inspected.
/// @return The port number associated with the address contained in ss.
uint16_t get_port(const sockaddr_storage *ss);
}
}  // namespace calebrjc::net::detail
