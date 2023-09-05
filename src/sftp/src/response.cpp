#include "sftp/response.hpp"

sftp_response sftp_response::success(std::string_view message) {
    return sftp_response{sftp_response_type::SUCCESS, std::string{message}};
}

sftp_response sftp_response::error(std::string_view message) {
    return sftp_response{sftp_response_type::ERROR, std::string{message}};
}

sftp_response sftp_response::number(uint64_t n) {
    return sftp_response{sftp_response_type::NUMBER, std::to_string(n)};
}

sftp_response sftp_response::logged_in(std::string_view message) {
    return sftp_response{sftp_response_type::LOGGED_IN, std::string{message}};
}
