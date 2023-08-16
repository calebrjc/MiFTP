#include "calebrjc/net/detail/socket_ops.hpp"

#include <unistd.h>

namespace calebrjc::net::detail::socket_ops {

namespace detail {

endpoint get_endpoint(socket_type socket_fd, bool local_socket) {
    // Get address
    address_storage_type ss;
    address_size_type ss_size = sizeof(ss);

    int getname_result = 0;

    if (local_socket) {
        getname_result = ::getsockname(socket_fd, (address_type *)&ss, &ss_size);
    } else {
        getname_result = ::getpeername(socket_fd, (address_type *)&ss, &ss_size);
    }

    //? Note(Caleb): We should probably check for errors here. Maybe use error_code?
    (void)getname_result;

    // Get socket protocol
    int protocol                    = 0;
    address_size_type protocol_size = sizeof(protocol);
    int gso_result = getsockopt(socket_fd, SOL_SOCKET, SO_PROTOCOL, &protocol, &protocol_size);

    //? Note(Caleb): We should probably check for errors here. Maybe use error_code?
    (void)gso_result;

    return endpoint::from_native_address(protocol, (address_type *)&ss, ss_size);
}

}  // namespace detail

endpoint get_local_endpoint(socket_type socket_fd) {
    return detail::get_endpoint(socket_fd, true);
}

endpoint get_remote_endpoint(socket_type socket_fd) {
    return detail::get_endpoint(socket_fd, false);
}

void close_socket(socket_type socket_fd) {
    // Note(Caleb): shutdown() does not fail meaningfully for our use cases, so we don't do any
    // error checking here.
    ::shutdown(socket_fd, SHUT_RDWR);

    // Note(Caleb): See above comment.
    ::close(socket_fd);
}

}  // namespace calebrjc::net::detail::socket_ops
