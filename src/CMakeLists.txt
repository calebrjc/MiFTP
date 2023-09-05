add_subdirectory(sftp)

add_executable(
    miftp_client
    "${CMAKE_CURRENT_SOURCE_DIR}/miftp_client.cpp"    
)
target_link_libraries(miftp_client sftp)
target_compile_features(miftp_client PRIVATE cxx_std_17)
target_compile_options(miftp_client PRIVATE -Wall -Wextra --pedantic-errors)

add_executable(
    miftp_server 
    "${CMAKE_CURRENT_SOURCE_DIR}/miftp_server.cpp"
)
target_link_libraries(miftp_server sftp)
target_compile_features(miftp_server PRIVATE cxx_std_17)
target_compile_options(miftp_server PRIVATE -Wall -Wextra --pedantic-errors)
