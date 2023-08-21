include(FetchContent)

# Add Catch2
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0
)
FetchContent_MakeAvailable(Catch2)

# Create tests target
set(TESTS
    "net/acceptor.test.cpp"
    "net/connection.test.cpp"
    "net/resolve.test.cpp"
    "net/detail/poll.test.cpp"
    "net/detail/socket_ops.test.cpp"
    "net/test_utils/test_utils.hpp"
    "net/test_utils/test_utils.cpp")

add_executable(test_runner ${TESTS})
target_link_libraries(test_runner PRIVATE net Catch2::Catch2WithMain pthread)
target_compile_options(test_runner PRIVATE -g -Wall -Wextra --pedantic-errors)

# Automatically register Catch2 tests
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(test_runner)
