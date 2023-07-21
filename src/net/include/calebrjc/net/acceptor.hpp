#pragma once

#include "connection.hpp"
#include "endpoint.hpp"

namespace calebrjc::net {
class Acceptor {
   public:
    Acceptor();
    Acceptor(Endpoint local_endpoint);

    ~Acceptor();

    Acceptor(const Acceptor &other)               = delete;
    Acceptor &operator=(const Acceptor &other)    = delete;
    Acceptor(const Connection &&other)            = delete;
    Acceptor &operator=(const Connection &&other) = delete;

    void open(Endpoint local_endpoint);
    void close();

    bool is_open();
    bool is_pending();

    Connection accept();
    void wait();
};
}  // namespace calebrjc::net
