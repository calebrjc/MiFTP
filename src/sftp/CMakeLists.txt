set(
    SFTP_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/sftp/request.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/sftp/response.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/sftp/server.hpp"
)

set(
    SFTP_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/request.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/response.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/server.cpp"
)

# Create library
add_library(sftp ${SFTP_HEADERS} ${SFTP_SOURCES})
target_include_directories(sftp PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(
    sftp
    PUBLIC yonaa
)
target_compile_features(sftp PUBLIC cxx_std_17)
target_compile_options(sftp PRIVATE -Wall -Wextra --pedantic-errors)
