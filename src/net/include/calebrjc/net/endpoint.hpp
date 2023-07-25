#pragma once

#include <string>

namespace calebrjc::net {
struct Endpoint {
    /// @brief Create an empty Endpoint.
    Endpoint();

    /// @brief Return the IP address referred to by this Endpoint in string form.
    /// @return The IP address referred to by this endpoint in string form.
    std::string addr() const;

    /// @brief Return the port number referred to by this Endpoint in string form.
    /// @return The port number referred to by this endpoint in string form.
    std::string port() const;

    /// @brief Return a string representation of this Endpoint.
    /// @return A string representation of this Endpoint.
    std::string str() const;
};
}  // namespace calebrjc::net
