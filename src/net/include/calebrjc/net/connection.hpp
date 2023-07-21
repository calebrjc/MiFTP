#pragma once

#include "buffer.hpp"
#include "endpoint.hpp"

namespace calebrjc::net {
class Connection {
   public:
    Connection();
    Connection(Endpoint remote_endpoint);

    ~Connection();

    Connection(const Connection &other)            = delete;
    Connection &operator=(const Connection &other) = delete;

    Connection(const Connection &&other)            = delete;
    Connection &operator=(const Connection &&other) = delete;

    void open(Endpoint remote_endpoint);
    void close();

    bool send(Buffer data);
    Buffer recv();

    bool is_open();

    Endpoint local_endpoint();
    Endpoint remote_endpoint();
};
}
