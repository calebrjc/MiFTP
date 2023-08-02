add_subdirectory(net)

add_executable(miftp miftp.cpp)
target_compile_features(miftp PRIVATE cxx_std_17)
target_compile_options(miftp PRIVATE -Wall -Wextra --pedantic-errors)
