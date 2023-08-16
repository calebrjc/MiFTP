#include "calebrjc/net/detail/poll.hpp"

#include <poll.h>

namespace calebrjc::net::detail::poll {

static const int poll_config_mask = POLLIN | POLLOUT;

namespace detail {

/// @brief Return a socket_status_mask that represents the status described by revents.
/// @param revents The events to convert.
/// @return A socket_status_mask that represents the status described by revents.
socket_status_mask ssm_from_revents(int revents) {
    socket_status_mask ssm;

    ssm |= (revents & POLLIN) ? socket_status::readable : socket_status::none;
    ssm |= (revents & POLLOUT) ? socket_status::writable : socket_status::none;
    ssm |= (revents & POLLERR) ? socket_status::error : socket_status::none;
    ssm |= (revents & POLLHUP) ? socket_status::hung_up : socket_status::none;

    return ssm;
}

}  // namespace detail

socket_status_mask poll_socket(socket_type socket_fd, int timeout_millis) {
    pollfd pollfd = {0};
    pollfd.fd     = socket_fd;
    pollfd.events = poll_config_mask;

    int events_fired = ::poll(&pollfd, 1, timeout_millis);

    return (events_fired > 0) ? detail::ssm_from_revents(pollfd.revents) : socket_status::none;
}

void poll_group::add_socket(socket_type socket_fd) {}

void poll_group::remove_socket(socket_type socket_fd) {}

poll_result poll_group::poll() {
    return {};
}

}  // namespace calebrjc::net::detail::poll
