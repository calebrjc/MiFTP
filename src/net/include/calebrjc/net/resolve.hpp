#pragma once

#include <system_error>
#include <vector>

#include "endpoint.hpp"

namespace calebrjc::net {
/// @brief The type returned by a call to `net::resolve()`.
using ResolveResult = std::vector<Endpoint>;

/// @brief The hostname used to refer to the host computer in calls to `net::resolve()` (see: INADDR_ANY).
const std::string all_local_addresses = "0.0.0.0";

/// @brief Return the result of name resolution for the given hostname and service.
/// @param hostname The IP (v4 or v6) address of the desired host, or their canonical name.
/// @param service The name of the desired service or its corresponding port number, in string form.
/// @return The result of name resolution for the given hostname and service.
ResolveResult resolve(std::string hostname, std::string service);

/// @brief Return the result of name resolution for the given hostname and service.
/// @param hostname The IP (v4 or v6) address of the desired host, or their canonical name.
/// @param service The name of the desired service or its corresponding port number, in string form.
/// @param ec An error_code that is set if an error occurs.
/// @return The result of name resolution for the given hostname and service.
ResolveResult resolve(std::string hostname, std::string service, std::error_code &ec);
}  // namespace calebrjc::net
