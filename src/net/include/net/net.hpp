#pragma once

#include <string>

#include "buffer.hpp"

namespace calebrjc::net {
struct Endpoint {
    Endpoint();
    Endpoint(std::string hostname, std::string service);
    Endpoint(std::string hostname, uint16_t port_number);
    Endpoint(std::string service);
    Endpoint(uint16_t port_number);

    std::string addr() const;
    std::string port() const;
    std::string str() const;
};

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
