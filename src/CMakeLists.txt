set(MIFTP_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/miftp/sftp_server.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/miftp/sftp_request.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/miftp/sftp_response.hpp"
)

set(MIFTP_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/miftp.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/miftp/sftp_server.cpp"
)

add_executable(miftp ${MIFTP_HEADERS} ${MIFTP_SOURCES})
target_link_libraries(miftp yonaa)
target_compile_features(miftp PRIVATE cxx_std_17)
target_compile_options(miftp PRIVATE -Wall -Wextra --pedantic-errors)
