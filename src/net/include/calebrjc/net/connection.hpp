#pragma once

#include <system_error>

#include "buffer.hpp"
#include "endpoint.hpp"
#include "resolve.hpp"

namespace calebrjc::net {
class Connection {
   public:
    /// @brief Create an unopened Connection.
    Connection();

    /// @brief Create a Connection, bound to a local endpoint.
    /// @param local_endpoints The resolved local endpoint that this Connection will be bound to.
    Connection(ResolveResult local_endpoint);

    /// @brief Cleanup a Connection.
    ~Connection();

    // Disable copies ------------------------------------------------------------------------------

    Connection(const Connection &other)            = delete;
    Connection &operator=(const Connection &other) = delete;

    // ---------------------------------------------------------------------------------------------

    /// @brief Create a Connection from another Connection.
    /// @param other The other Connection.
    Connection(const Connection &&other);

    /// @brief Create a Connection from another Connection.
    /// @param other The other Connection.
    Connection &operator=(const Connection &&other);

    /// @brief Bind the local end of this Connection to the given local endpoint.
    /// @param local_endpoint The resolved local endpoint that this Connection will be bound to.
    void bind(ResolveResult local_endpoint);

    /// @brief Bind the local end of this Connection to the given local endpoint.
    /// @param local_endpoint The resolved local endpoint that this Connection will be bound to.
    /// @param ec An error_code that is set if an error occurs.
    void bind(ResolveResult local_endpoint, std::error_code &ec);

    /// @brief Establish a connection to the given remote endpoint.
    /// @param remote_endpoint The resolved remote endpoint that this Connection will connect to.
    void connect(ResolveResult remote_endpoint);

    /// @brief Establish a connection to the given remote endpoint.
    /// @param remote_endpoint The resolved remote endpoint that this Connection will connect to.
    /// @param ec An error_code that is set if an error occurs.
    void connect(ResolveResult remote_endpoint, std::error_code &ec);

    /// @brief Close this Connection gracefully.
    void disconnect();

    /// @brief Close this Connection gracefully.
    /// @param ec An error_code that is set if an error occurs.
    void disconnect(std::error_code &ec);

    /// @brief Send the data contained in the given Buffer to the remote endpoint of this
    /// Connection.
    /// @param data The data to be sent.
    void send(Buffer data) const;

    /// @brief Send the data contained in the given Buffer to the remote endpoint of this
    /// Connection.
    /// @param data The data to be sent.
    /// @param ec An error_code that is set if an error occurs.
    void send(Buffer data, std::error_code &ec) const;

    /// @brief Return a Buffer containing data send from the remote endpoint of this connection.
    /// @return A Buffer containing data send from the remote endpoint of this connection.
    Buffer receive() const;

    /// @brief Return a Buffer containing data send from the remote endpoint of this connection.
    /// @return A Buffer containing data send from the remote endpoint of this connection.
    /// @param ec An error_code that is set if an error occurs.
    Buffer receive(std::error_code &ec) const;

    /// @brief Return true if this Connection has established a connection with a remote endpoint.
    /// @return True if this Connection has established a connection with a remote endpoint.
    bool is_open() const;

    /// @brief Return the local endpoint that this Connection is bound to.
    /// @return The local endpoint that this Connection is bound to.
    Endpoint local_endpoint() const;

    /// @brief Return the remote endpoint that this Connection is connected to. Invalid if no
    /// connection is established.
    /// @return The remote endpoint that this Connection is connected to.
    Endpoint remote_endpoint() const;
};
}  // namespace calebrjc::net
