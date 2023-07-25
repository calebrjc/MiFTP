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
    "net/test_resolve.cpp")

add_executable(tests ${TESTS})
target_link_libraries(tests PRIVATE net Catch2::Catch2WithMain)

# Automatically register Catch2 tests
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(tests)
