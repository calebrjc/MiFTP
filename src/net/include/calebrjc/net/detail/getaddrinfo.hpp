#pragma once

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <string>

namespace calebrjc::net::detail {
/// @brief Return the addrinfo * from a call to getaddrinfo() with the name and service specified.
/// The pointer will have to be freed with freeaddrinfo().
/// @param name The name to be resolved.
/// @param service The service to be resolved.
/// @return The addrinfo * from a call to getaddrinfo() with the name and service specified.
addrinfo *getaddrinfo(std::string hostname, std::string service);
}  // namespace calebrjc::net::detail
