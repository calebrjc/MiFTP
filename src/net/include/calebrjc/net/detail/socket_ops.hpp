#pragma once

#include "calebrjc/net/endpoint.hpp"

namespace calebrjc::net::detail {
namespace socket_ops {
// TODO(Caleb): Document detail namespaces.

endpoint get_local_endpoint(int socket_fd);
endpoint get_remote_endpoint(int socket_fd);
bool is_ready_to_read(int socket_fd, int timeout_millis);
}
}  // namespace calebrjc::net::detail
