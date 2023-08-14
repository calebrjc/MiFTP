#pragma once

#include <system_error>

#include "bitmask/bitmask.hpp"
#include "calebrjc/net/buffer.hpp"
#include "calebrjc/net/endpoint.hpp"
#include "calebrjc/net/resolve.hpp"

namespace calebrjc::net {

/// @brief Flags used to customize calls to Connection::send().
enum class send_flags {
    none = 0,

    /// @brief A flag that specifies that the sent data should not be subject to routing.
    dont_route = 0x01,

    /// @brief A flag that specifies that the sent data should not be subject to routing.
    end_of_record = 0x02,
};
BITMASK_DEFINE_MAX_ELEMENT(send_flags, end_of_record)
using send_flags_mask = bitmask::bitmask<send_flags>;

/// @brief Flags used to customize calls to Connection::receive().
enum class receive_flags {
    none = 0,

    /// @brief A flag that specifies that the received data should be returned, but not taken off of
    /// the input queue.
    peek = 0x01,
};
BITMASK_DEFINE_MAX_ELEMENT(receive_flags, peek)
using receive_flags_mask = bitmask::bitmask<receive_flags>;

/// @brief A networking entity that allows communication between the host and another endpoint,
/// local or remote.
class Connection {
   public:
    /// @brief Create an unopened Connection.
    Connection();

    /// @brief Return an open Connection created from a connected native socket.
    /// @param socket_fd The open native socket's file descriptor.
    /// @param remote_endpoint An Endpoint representing the remote end of the Connection.
    /// @return An open Connection created from an open native socket.
    static Connection from_native_socket(int socket_fd, Endpoint remote_endpoint);

    /// @brief Cleanup a Connection.
    ~Connection();

    // Disable copies ------------------------------------------------------------------------------

    Connection(const Connection &other)            = delete;
    Connection &operator=(const Connection &other) = delete;

    // ---------------------------------------------------------------------------------------------

    /// @brief Move a Connection from another Connection.
    /// @param other The other Connection.
    Connection(Connection &&other);

    /// @brief Move a Connection from another Connection.
    /// @param other The other Connection.
    Connection &operator=(Connection &&other);

   public:
    /// @brief Establish a connection to the resolved remote address.
    /// @param remote_endpoints The resolved remote address that this Connection will attempt to
    /// connect to.
    void connect(ResolveResult remote_endpoints);

    /// @brief Establish a connection to the resolved remote address.
    /// @param remote_endpoints The resolved remote address that this Connection will attempt to
    /// connect to.
    /// @param ec An error_code that is set if an error occurs.
    void connect(ResolveResult remote_endpoints, std::error_code &ec);

    /// @brief Close this Connection gracefully.
    void disconnect();

    /// @brief Send the data contained in the given Buffer to the remote endpoint of this
    /// Connection.
    /// @param data The data to be sent.
    /// @param flags A bitfield of send_flags constants used to customize this call to send().
    void send(const Buffer &data, send_flags_mask flags = send_flags::none) const;

    /// @brief Send the data contained in the given Buffer to the remote endpoint of this
    /// Connection.
    /// @param data The data to be sent.
    /// @param ec An error_code that is set if an error occurs.
    void send(const Buffer &data, std::error_code &ec) const;

    /// @brief Send the data contained in the given Buffer to the remote endpoint of this
    /// Connection.
    /// @param data The data to be sent.
    /// @param flags A bitfield of send_flags constants used to customize this call to send().
    /// @param ec An error_code that is set if an error occurs.
    void send(const Buffer &data, send_flags_mask flags, std::error_code &ec) const;

    /// @brief Return a Buffer containing data send from the remote endpoint of this connection.
    /// @return A Buffer containing data send from the remote endpoint of this connection.
    /// @param flags A bitfield of receive_flags constants used to customize this call to
    /// receive().
    Buffer receive(receive_flags_mask flags = receive_flags::none) const;

    /// @brief Return a Buffer containing data send from the remote endpoint of this connection.
    /// @return A Buffer containing data send from the remote endpoint of this connection.
    /// @param ec An error_code that is set if an error occurs.
    Buffer receive(std::error_code &ec) const;

    /// @brief Return a Buffer containing data send from the remote endpoint of this connection.
    /// @return A Buffer containing data send from the remote endpoint of this connection.
    /// @param flags A bitfield of receive_flags constants used to customize this call to
    /// receive().
    /// @param ec An error_code that is set if an error occurs.
    Buffer receive(receive_flags_mask flags, std::error_code &ec) const;

    /// @brief Return true if this Connection has established a connection with a remote endpoint.
    /// @return True if this Connection has established a connection with a remote endpoint.
    bool is_connected() const;

    /// @brief Return the local endpoint that this Connection is bound to. Invalid if no
    /// connection is established.
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
