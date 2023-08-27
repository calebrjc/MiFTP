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
    "${CMAKE_CURRENT_SOURCE_DIR}/miftp.test.cpp")

add_executable(miftp_tests ${TESTS})
target_link_libraries(miftp_tests PRIVATE yonaa Catch2::Catch2WithMain pthread)
target_compile_options(miftp_tests PRIVATE -g -Wall -Wextra --pedantic-errors)

# Automatically register Catch2 tests
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(miftp_tests)
