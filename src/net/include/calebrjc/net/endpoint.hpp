#pragma once

#include <string>

namespace calebrjc::net {
class Endpoint {
   public:
    Endpoint();
    Endpoint(std::string hostname, std::string service);
    Endpoint(std::string hostname, uint16_t port_number);
    Endpoint(std::string service);
    Endpoint(uint16_t port_number);

    std::string addr() const;
    std::string port() const;
    std::string str() const;
};
}  // namespace calebrjc::net
