#include "calebrjc/net/resolve.hpp"

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <cstring>
#include <iostream>

#include "calebrjc/net/addresses.hpp"
#include "calebrjc/net/detail/getaddrinfo.hpp"

namespace calebrjc::net {
ResolveResult resolve(std::string hostname, std::string service) {
    // Delegate function call and throw if necessary
    std::error_code ec;
    auto result = resolve(hostname, service, ec);

    if (ec) throw ec;

    return result;
}

ResolveResult resolve(std::string hostname, std::string service, std::error_code &ec) {
    addrinfo *target_info = detail::getaddrinfo(hostname, service);
    if (!target_info) {
        // TODO(Caleb): Error handling here
        ec.assign(3, std::system_category());
        return {};
    }

    // Pack result
    ResolveResult result;
    for (addrinfo *ai = target_info; ai != nullptr; ai = ai->ai_next) {
        auto e = Endpoint::from_native_address(ai->ai_protocol, ai->ai_addr, ai->ai_addrlen);
        result.push_back(e);
    }

    freeaddrinfo(target_info);

    return result;
}
}  // namespace calebrjc::net
