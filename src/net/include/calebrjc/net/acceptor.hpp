#pragma once

#include <system_error>

#include "connection.hpp"
#include "resolve.hpp"

namespace calebrjc::net {
class Acceptor {
   public:
    /// @brief Create an unopened and unbound Acceptor.
    Acceptor();

    /// @brief Cleanup an Acceptor.
    ~Acceptor();

    // Disable copies and moves --------------------------------------------------------------------

    Acceptor(const Acceptor &other)               = delete;
    Acceptor &operator=(const Acceptor &other)    = delete;
    Acceptor(const Connection &&other)            = delete;
    Acceptor &operator=(const Connection &&other) = delete;

    // ---------------------------------------------------------------------------------------------

    /// @brief Bind this Acceptor and open it to incoming connections.
    /// @param local_endpoints The resolved local address.
    void open(ResolveResult local_endpoint);

    /// @brief Bind this Acceptor and open it to incoming connections.
    /// @param local_endpoints The resolved local address.
    /// @param ec An error_code that is set if an error occurs.
    void open(ResolveResult local_endpoint, std::error_code &ec);

    /// @brief Close this Acceptor from incoming connections.
    void close();

    /// @brief Close this acceptor from incoming connections.
    /// @param ec An error_code that is set if an error occurs.
    void close(std::error_code &ec);

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
};
}  // namespace calebrjc::net
