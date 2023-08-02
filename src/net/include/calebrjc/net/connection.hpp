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

    /// @brief Return an open Connection created from an open native socket.
    /// @param socket_fd The open native socket.
    /// @return An open Connection created from an open native socket.
    static Connection from_fd(int socket_fd);

    /// @brief Cleanup a Connection.
    ~Connection();

    // Disable copies ------------------------------------------------------------------------------

    Connection(const Connection &other)            = delete;
    Connection &operator=(const Connection &other) = delete;

    // ---------------------------------------------------------------------------------------------

    /// @brief Create a Connection from another Connection.
    /// @param other The other Connection.
    Connection(Connection &&other);

    /// @brief Create a Connection from another Connection.
    /// @param other The other Connection.
    Connection &operator=(Connection &&other);

    /// @brief Establish a connection to the given remote endpoint.
    /// @param remote The resolved remote endpoint that this Connection will connect to.
    void connect(ResolveResult remote);

    /// @brief Establish a connection to the given remote endpoint.
    /// @param remote The resolved remote endpoint that this Connection will connect to.
    /// @param ec An error_code that is set if an error occurs.
    void connect(ResolveResult remote, std::error_code &ec);

    /// @brief Close this Connection gracefully.
    void disconnect();

    /// @brief Send the data contained in the given Buffer to the remote endpoint of this
    /// Connection.
    /// @param data The data to be sent.
    void send(const Buffer &data) const;

    /// @brief Send the data contained in the given Buffer to the remote endpoint of this
    /// Connection.
    /// @param data The data to be sent.
    /// @param ec An error_code that is set if an error occurs.
    void send(const Buffer &data, std::error_code &ec) const;

    /// @brief Return a Buffer containing data send from the remote endpoint of this connection.
    /// @return A Buffer containing data send from the remote endpoint of this connection.
    Buffer receive() const;

    /// @brief Return a Buffer containing data send from the remote endpoint of this connection.
    /// @return A Buffer containing data send from the remote endpoint of this connection.
    /// @param ec An error_code that is set if an error occurs.
    Buffer receive(std::error_code &ec) const;

    /// @brief Return true if this Connection has established a connection with a remote endpoint.
    /// @return True if this Connection has established a connection with a remote endpoint.
    bool is_connected() const;

    /// @brief Return the local endpoint that this Connection is bound to.
    /// @return The local endpoint that this Connection is bound to.
    Endpoint local_endpoint() const;

    /// @brief Return the remote endpoint that this Connection is connected to. Invalid if no
    /// connection is established.
    /// @return The remote endpoint that this Connection is connected to.
    Endpoint remote_endpoint() const;

   private:
    int socket_;
    Endpoint local_endpoint_;
    Endpoint remote_endpoint_;
};
}  // namespace calebrjc::net
