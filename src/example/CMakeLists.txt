# Example echo client
add_executable(echo_client echo_client.cpp)
target_link_libraries(echo_client PRIVATE net)
target_compile_features(echo_client PUBLIC cxx_std_17)
target_compile_options(echo_client PRIVATE -Wall -Wextra --pedantic-errors)

# Example echo server
add_executable(echo_server echo_server.cpp)
target_link_libraries(echo_server PRIVATE net)
target_compile_features(echo_server PUBLIC cxx_std_17)
target_compile_options(echo_server PRIVATE -Wall -Wextra --pedantic-errors)
