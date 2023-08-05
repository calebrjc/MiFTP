#pragma once

#include <arpa/inet.h>

namespace calebrjc::net::detail::sockaddr_ops {
/// @brief Return the in_addr or in6_addr associated with the address contained in ss.
/// @param sa The address to be inspected.
/// @return The in_addr or in6_addr associated with the address contained in ss.
void *get_in_addr(const sockaddr *sa);

/// @brief Return the port number associated with the address contained in ss.
/// @param sa The address to be inspected.
/// @return The port number associated with the address contained in ss.
uint16_t get_port(const sockaddr *sa);
}  // namespace calebrjc::net::detail
