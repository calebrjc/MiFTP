#pragma once

#include "calebrjc/net/endpoint.hpp"
#include "calebrjc/net/types.hpp"

namespace calebrjc::net::detail::socket_ops {

/// @brief Return an endpoint representing the local end of this socket.
/// @param socket_fd The socket to query.
/// @return An endpoint representing the local end of this socket.
endpoint get_local_endpoint(socket_type socket_fd);

/// @brief Return an endpoint representing the remote end of this socket.
/// @param socket_fd The socket to query.
/// @return An endpoint representing the remote end of this socket.
endpoint get_remote_endpoint(socket_type socket_fd);

/// @brief Gracefully close an open socket.
/// @param socket_fd The socket to close.
void close_socket(socket_type socket_fd);

}  // namespace calebrjc::net::detail
