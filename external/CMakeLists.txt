include(FetchContent)

FetchContent_Declare(
  yonaa
  GIT_REPOSITORY https://github.com/calebrjc/yonaa.git
  GIT_TAG        trunk

  PREFIX         yonaa
  SOURCE_DIR     ${PROJECT_SOURCE_DIR}/external/yonaa
)
FetchContent_MakeAvailable(yonaa)
