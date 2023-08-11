set(HEADERS 
    "include/calebrjc/net/net.hpp"
    "include/calebrjc/net/acceptor.hpp"
    "include/calebrjc/net/addresses.hpp"
    "include/calebrjc/net/buffer.hpp"
    "include/calebrjc/net/connection.hpp"
    "include/calebrjc/net/endpoint.hpp"
    "include/calebrjc/net/resolve.hpp"
    "include/calebrjc/net/detail/flag_set.hpp"
    "include/calebrjc/net/detail/getaddrinfo.hpp"
    "include/calebrjc/net/detail/sockaddr_ops.hpp"
    "include/calebrjc/net/detail/socket_ops.hpp")

set(SOURCES
    "acceptor.cpp"
    "connection.cpp"
    "endpoint.cpp"
    "resolve.cpp"
    "detail/getaddrinfo.cpp"
    "detail/sockaddr_ops.cpp"
    "detail/socket_ops.cpp")

add_library(net ${SOURCES} ${HEADERS})
target_include_directories(net PUBLIC include)
target_compile_features(net PUBLIC cxx_std_17)
