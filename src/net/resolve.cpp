#include "calebrjc/net/resolve.hpp"

namespace calebrjc::net {
ResolveResult resolve(std::string_view hostname, std::string_view service) {
    (void)hostname;
    (void)service;

    return {};
}

ResolveResult resolve(std::string_view hostname, std::string_view service, std::error_code &ec) {
    (void)hostname;
    (void)service;
    (void)ec;

    return {};
}
}  // namespace calebrjc::net
