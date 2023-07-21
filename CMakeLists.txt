cmake_minimum_required(VERSION 3.16)

project(
    MiFTP
    DESCRIPTION "A minimal implementation of TFTP."
    LANGUAGES C CXX
)

# Let's ensure -std=c++xx instead of -std=g++xx
set(CMAKE_CXX_EXTENSIONS OFF)

# Let's nicely support folders in IDEs
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(src)

# # Create server and client executables
# add_executable(miftp src/miftp.cpp)
# target_include_directories(miftp PUBLIC src/miftp)
# target_compile_features(miftp PUBLIC cxx_std_17)

# add_executable(echo_client src/miftp/util/echo_client.cpp src/miftp/net.cpp)
# target_include_directories(echo_client PUBLIC src/miftp)
# target_compile_features(echo_client PUBLIC cxx_std_17)
# target_compile_options(echo_client PRIVATE -Wall -Wextra --pedantic-errors)

# add_executable(echo_server src/miftp/util/echo_server.cpp src/miftp/net.cpp)
# target_include_directories(echo_server PUBLIC src/miftp)
# target_compile_features(echo_server PUBLIC cxx_std_17)
# target_compile_options(echo_server PRIVATE -Wall -Wextra --pedantic-errors)
