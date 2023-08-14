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

# Add libs
add_subdirectory(external)

# Add code
add_subdirectory(src)

# Conditionally enable tests
if(BUILD_TESTING)
    add_subdirectory(test)
    enable_testing()
endif()
