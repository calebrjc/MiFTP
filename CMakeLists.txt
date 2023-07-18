cmake_minimum_required(VERSION 3.16)

project(
    MiFTP
    DESCRIPTION "A minimal implementation of TFTP."
    LANGUAGES C CXX
)

# Create server and client executables
add_executable(miftp src/miftp.cpp src/miftp/util/echo_client.cpp)
target_include_directories(miftp PUBLIC src/miftp)
target_compile_features(miftp PUBLIC cxx_std_17)
