#pragma once

#include <system_error>

#include "buffer.hpp"
#include "endpoint.hpp"
#include "resolve.hpp"

namespace calebrjc::net {
namespace SendFlags {
/// @brief A flag that specifies that the sent data should not be subject to routing.
static const uint32_t dont_route = MSG_DONTROUTE;

/// @brief A flag that specifies that the sent data marks the end of a record.
static const uint32_t end_of_record = MSG_EOR;
}  // namespace SendFlags

namespace ReceiveFlags {
/// @brief A flag that specifies that the received data should be returned, but not taken off of the
/// input queue.
static const uint32_t peek = MSG_PEEK;
}  // namespace ReceiveFlags

class Connection {
   public:
    /// @brief Create an unopened Connection.
    Connection();

    /// @brief Return an open Connection created from an open native socket.
    /// @param socket_fd The open native socket.
    /// @param remote_endpoint An Endpoint representing the remote end of the Connection.
    /// @return An open Connection created from an open native socket.
    static Connection from_fd(int socket_fd, Endpoint remote_endpoint);

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
    /// @param flags A bitfield of SendFlags constants used to customize this call to `send()`.
    void send(const Buffer &data, uint32_t flags = 0) const;

    /// @brief Send the data contained in the given Buffer to the remote endpoint of this
    /// Connection.
    /// @param data The data to be sent.
    /// @param ec An error_code that is set if an error occurs.
    void send(const Buffer &data, std::error_code &ec) const;

    /// @brief Send the data contained in the given Buffer to the remote endpoint of this
    /// Connection.
    /// @param data The data to be sent.
    /// @param flags A bitfield of SendFlags constants used to customize this call to `send()`.
    /// @param ec An error_code that is set if an error occurs.
    void send(const Buffer &data, uint32_t flags, std::error_code &ec) const;

    /// @brief Return a Buffer containing data send from the remote endpoint of this connection.
    /// @return A Buffer containing data send from the remote endpoint of this connection.
    /// @param flags A bitfield of ReceiveFlags constants used to customize this call to
    /// `receive()`.
    Buffer receive(uint32_t flags = 0) const;

    /// @brief Return a Buffer containing data send from the remote endpoint of this connection.
    /// @return A Buffer containing data send from the remote endpoint of this connection.
    /// @param ec An error_code that is set if an error occurs.
    Buffer receive(std::error_code &ec) const;

    /// @brief Return a Buffer containing data send from the remote endpoint of this connection.
    /// @return A Buffer containing data send from the remote endpoint of this connection.
    /// @param flags A bitfield of ReceiveFlags constants used to customize this call to
    /// `receive()`.
    /// @param ec An error_code that is set if an error occurs.
    Buffer receive(uint32_t flags, std::error_code &ec) const;

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
