set(HEADERS 
    "include/net/buffer.hpp"
    "include/net/net.hpp")

add_library(net net.cpp ${HEADERS})
target_include_directories(net PUBLIC include)
target_compile_features(net PUBLIC cxx_std_17)

source_group(
  TREE "${PROJECT_SOURCE_DIR}/src/net/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})

