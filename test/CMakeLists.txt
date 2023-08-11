include(FetchContent)

# Add Catch2
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0
)
FetchContent_MakeAvailable(Catch2)

# Example echo client
add_executable(simple_echo_client simple_echo_client.cpp)
target_link_libraries(simple_echo_client PRIVATE net)
target_compile_features(simple_echo_client PUBLIC cxx_std_17)
target_compile_options(simple_echo_client PRIVATE -Wall -Wextra --pedantic-errors)

# Example echo server
add_executable(simple_echo_server simple_echo_server.cpp)
target_link_libraries(simple_echo_server PRIVATE net)
target_compile_features(simple_echo_server PUBLIC cxx_std_17)
target_compile_options(simple_echo_server PRIVATE -Wall -Wextra --pedantic-errors)

# Create tests target
set(TESTS
    "net/test_resolve.cpp"
    "net/test_everything_else.cpp")

add_executable(test_runner ${TESTS})
target_link_libraries(test_runner PRIVATE net Catch2::Catch2WithMain pthread)

# Automatically register Catch2 tests
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(test_runner)
