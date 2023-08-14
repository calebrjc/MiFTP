#pragma once

#include <system_error>

#include "connection.hpp"
#include "resolve.hpp"

namespace calebrjc::net {
/// @brief A networking entity that allows a host to listen for and accept incoming connections.
class acceptor {
   public:
    /// @brief Create an unopened and unbound acceptor.
    acceptor();

    /// @brief Cleanup after an acceptor.
    ~acceptor();

    // Disable copies and moves --------------------------------------------------------------------

    acceptor(const acceptor &other)             = delete;
    acceptor &operator=(const acceptor &other)  = delete;
    acceptor(const acceptor &&other)            = delete;
    acceptor &operator=(const acceptor &&other) = delete;

    // ---------------------------------------------------------------------------------------------

    /// @brief Bind this acceptor to the resolved local address and open it to incoming connections.
    /// @param local_endpoints The resolved local address.
    /// @param reuse_addr True if the SO_REUSEADDR option should be enabled for this acceptor, and
    /// false otherwise (see: man 7 ip).
    void open(resolve_result local_endpoints, bool reuse_addr = false);

    /// @brief Bind this acceptor to the resolved local address and open it to incoming connections.
    /// @param local_endpoints The resolved local address.
    /// @param ec An error_code that is set if an error occurs.
    /// @param reuse_addr True if the SO_REUSEADDR option should be enabled for this acceptor, and
    /// false otherwise (see: man 7 ip).
    void open(resolve_result local_endpoints, std::error_code &ec, bool reuse_addr = false);

    /// @brief Stop this acceptor from accepting incoming connections and close it.
    void close();

    /// @brief Return true if this acceptor is open to incoming connections.
    /// @return True if this acceptor is open to incoming connections.
    bool is_open() const;

    /// @brief Return true if this acceptor has a connection waiting to be accepted.
    /// @return True if this acceptor has a connection waiting to be accepted.
    bool is_pending() const;

    /// @brief Return a connection to the remote attempting to connect to this acceptor.
    /// @return A connection to the remote attempting to connect to this acceptor.
    connection accept() const;

    /// @brief Return a connection to the remote attempting to connect to this acceptor.
    /// @return A connection to the remote attempting to connect to this acceptor.
    /// @param ec An error_code that is set if an error occurs.
    connection accept(std::error_code &ec) const;

    /// @brief Return the local endpoint that this acceptor is bound to. Invalid if this acceptor is
    /// not bound.
    /// @return The local endpoint that this acceptor is bound to.
    endpoint local_endpoint() const;

   private:
    int socket_;
    endpoint local_endpoint_;
};
}  // namespace calebrjc::net
