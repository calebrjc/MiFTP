set(HEADERS 
    "${CMAKE_CURRENT_SOURCE_DIR}/include/calebrjc/net/net.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/calebrjc/net/acceptor.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/calebrjc/net/addresses.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/calebrjc/net/buffer.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/calebrjc/net/connection.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/calebrjc/net/endpoint.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/calebrjc/net/resolve.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/calebrjc/net/types.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/calebrjc/net/app/server.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/calebrjc/net/detail/getaddrinfo.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/calebrjc/net/detail/poll.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/calebrjc/net/detail/sockaddr_ops.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/calebrjc/net/detail/socket_ops.hpp")

set(SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/acceptor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/connection.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/endpoint.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/resolve.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/app/server.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/detail/getaddrinfo.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/detail/poll.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/detail/sockaddr_ops.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/detail/socket_ops.cpp")

add_library(net ${SOURCES} ${HEADERS})
target_include_directories(net PUBLIC "include")
target_link_libraries(net PUBLIC bitmask spdlog)
target_compile_features(net PUBLIC cxx_std_17)
