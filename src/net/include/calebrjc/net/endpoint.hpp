#pragma once

#include <sys/socket.h>

#include <string>

namespace calebrjc::net {
struct Endpoint {
    /// @brief Create an empty Endpoint.
    Endpoint();

    /// @brief Return an Endpoint created from a native address structure.
    /// @param protocol The protocol associated with the native address structure. (see: man 7 ip)
    /// @param addr The native address structure.
    /// @param addr_size The size (in bytes) of the native address structure.
    /// @return An Endpoint created from a native address structure.
    static Endpoint from_native_address(int protocol, sockaddr *addr, socklen_t addr_size);

    /// @brief Return the identifier of the address family associated with this socket.
    /// @return The identifier of the address family associated with this socket.
    sa_family_t family() const;

    /// @brief Return the identifier of the protocol associated with this socket. (see: man 7 ip)
    /// @return The identifier of the protocol associated with this socket. (see: man 7 ip)
    int protocol() const;

    /// @brief Return a pointer to the underlying native storage for this Endpoint.
    /// @return A pointer to the underlying native storage for this Endpoint.
    sockaddr *data();

    /// @brief Return a pointer to the underlying native storage for this Endpoint.
    /// @return A pointer to the underlying native storage for this Endpoint.
    const sockaddr *data() const;

    socklen_t size() const;

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
    int protocol_;
    sockaddr_storage storage_;
    socklen_t storage_size_;
};
}  // namespace calebrjc::net
