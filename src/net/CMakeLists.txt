set(HEADERS 
    "include/calebrjc/net/net.hpp"
    "include/calebrjc/net/acceptor.hpp"
    "include/calebrjc/net/buffer.hpp"
    "include/calebrjc/net/connection.hpp"
    "include/calebrjc/net/endpoint.hpp")

set(SOURCES
    "acceptor.cpp"
    "connection.cpp"
    "endpoint.cpp")

add_library(net ${SOURCES} ${HEADERS})
target_include_directories(net PUBLIC include)
target_compile_features(net PUBLIC cxx_std_17)

source_group(
  TREE "${PROJECT_SOURCE_DIR}/src/net/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})

