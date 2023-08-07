#pragma once

#include <system_error>

#include "connection.hpp"
#include "resolve.hpp"

namespace calebrjc::net {
/// @brief A networking entity that allows a host to listen for and accept incoming Connections.
class Acceptor {
   public:
    /// @brief Create an unopened and unbound Acceptor.
    Acceptor();

    /// @brief Cleanup after an Acceptor.
    ~Acceptor();

    // Disable copies and moves --------------------------------------------------------------------

    Acceptor(const Acceptor &other)               = delete;
    Acceptor &operator=(const Acceptor &other)    = delete;
    Acceptor(const Connection &&other)            = delete;
    Acceptor &operator=(const Connection &&other) = delete;

    // ---------------------------------------------------------------------------------------------

    /// @brief Bind this Acceptor to the resolved local address and open it to incoming connections.
    /// @param local_endpoints The resolved local address.
    /// @param reuse_addr True if the SO_REUSEADDR option should be enabled for this acceptor, and
    /// false otherwise (see: man 7 ip).
    void open(ResolveResult local_endpoints, bool reuse_addr = false);

    /// @brief Bind this Acceptor to the resolved local address and open it to incoming connections.
    /// @param local_endpoints The resolved local address.
    /// @param ec An error_code that is set if an error occurs.
    /// @param reuse_addr True if the SO_REUSEADDR option should be enabled for this acceptor, and
    /// false otherwise (see: man 7 ip).
    void open(ResolveResult local_endpoints, std::error_code &ec, bool reuse_addr = false);

    /// @brief Stop this Acceptor from accepting incoming connections and close it.
    void close();

    /// @brief Return true if this Acceptor is open to incoming connections.
    /// @return True if this acceptor is open to incoming connections.
    bool is_open() const;

    /// @brief Return true if this Acceptor has a connection waiting to be accepted.
    /// @return True if this Acceptor has a connection waiting to be accepted.
    bool is_pending() const;

    /// @brief Return a Connection to the remote attempting to connect to this Acceptor.
    /// @return A Connection to the remote attempting to connect to this Acceptor.
    Connection accept() const;

    /// @brief Return a Connection to the remote attempting to connect to this Acceptor.
    /// @return A Connection to the remote attempting to connect to this Acceptor.
    /// @param ec An error_code that is set if an error occurs.
    Connection accept(std::error_code &ec) const;

    /// @brief Return the local endpoint that this Acceptor is bound to. Invalid if this Acceptor is
    /// not bound.
    /// @return The local endpoint that this Acceptor is bound to.
    Endpoint local_endpoint() const;

   private:
    int socket_;
    Endpoint local_endpoint_;
};
}  // namespace calebrjc::net
