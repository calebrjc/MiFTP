#include "calebrjc/net/detail/socket_ops.hpp"

#include <poll.h>

namespace calebrjc::net::detail::socket_ops {
namespace detail {
endpoint get_endpoint(int socket_fd, bool local_socket) {
    // Get address
    sockaddr_storage ss;
    socklen_t ss_size = sizeof(ss);

    int getname_result = 0;

    if (local_socket) {
        getname_result = ::getsockname(socket_fd, (sockaddr *)&ss, &ss_size);
    } else {
        getname_result = ::getpeername(socket_fd, (sockaddr *)&ss, &ss_size);
    }
    //? Note(Caleb): We should probably check for errors here. Maybe use error_code?

    // Get socket protocol
    int protocol            = 0;
    socklen_t protocol_size = sizeof(protocol);
    int gso_result = getsockopt(socket_fd, SOL_SOCKET, SO_PROTOCOL, &protocol, &protocol_size);
    //? Note(Caleb): We should probably check for errors here. Maybe use error_code?

    return endpoint::from_native_address(protocol, (sockaddr *)&ss, ss_size);
}
}  // namespace detail

endpoint get_local_endpoint(int socket_fd) {
    return detail::get_endpoint(socket_fd, true);
}

endpoint get_remote_endpoint(int socket_fd) {
    return detail::get_endpoint(socket_fd, false);
}

bool is_ready_to_read(int socket_fd, int timeout_millis) {
    pollfd pollfd;
    pollfd.fd = socket_fd;
    pollfd.events |= POLLIN;

    int events_fired = ::poll(&pollfd, 1, timeout_millis);
    return (events_fired > 0);
}
}  // namespace calebrjc::net::detail::socket_ops
