add_subdirectory(net)
add_subdirectory(util)

add_executable(miftp miftp.cpp)
target_compile_features(miftp PUBLIC cxx_std_17)
