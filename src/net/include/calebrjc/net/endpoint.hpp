#pragma once

#include <sys/socket.h>

#include <string>

namespace calebrjc::net {
struct Endpoint {
    /// @brief Create an empty Endpoint.
    Endpoint();

    /// @brief Return a pointer to the underlying native storage for this Endpoint.
    /// @return A pointer to the underlying native storage for this Endpoint.
    char *data() const;

    /// @brief Return the IP address referred to by this Endpoint in string form.
    /// @return The IP address referred to by this endpoint in string form.
    std::string addr() const;

    /// @brief Return the port number referred to by this Endpoint in string form.
    /// @return The port number referred to by this endpoint in string form.
    std::string port() const;

    /// @brief Return a string representation of this Endpoint.
    /// @return A string representation of this Endpoint.
    std::string str() const;

   private:
    sockaddr_storage storage_;
};
}  // namespace calebrjc::net
