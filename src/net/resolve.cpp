#include "calebrjc/net/resolve.hpp"

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <cstring>
#include <iostream>

namespace calebrjc::net {
ResolveResult resolve(std::string hostname, std::string service) {
    // Delegate function call and throw if necessary
    std::error_code ec;
    auto result = resolve(hostname, service, ec);

    if (ec) throw ec;

    return result;
}

ResolveResult resolve(std::string hostname, std::string service, std::error_code &ec) {
    // Should we be preparing to bind()?
    bool binding_address = (hostname == all_local_interfaces);

    // Prepare hints
    addrinfo hints;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_ADDRCONFIG | (binding_address) ? 0 : AI_PASSIVE;

    if (binding_address) { hints.ai_flags |= AI_PASSIVE; }

    // Perform resolution
    addrinfo *target_info;
    const char *name = (binding_address) ? NULL : hostname.c_str();
    int gai_result   = getaddrinfo(name, service.c_str(), &hints, &target_info);

    if (gai_result != 0) {
        ec.assign(gai_result, std::system_category());
        return {};
    }

    // Pack result
    ResolveResult result;
    for (addrinfo *ai = target_info; ai != nullptr; ai = ai->ai_next) {
        Endpoint e;
        std::memcpy(e.data(), ai->ai_addr, ai->ai_addrlen);
        result.push_back(e);
    }

    freeaddrinfo(target_info);

    return result;
}
}  // namespace calebrjc::net
