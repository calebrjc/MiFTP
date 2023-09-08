#include "sftp/response.hpp"

namespace sftp {

response response::success(std::string_view message) {
    return response{response_type::SUCCESS, std::string{message}};
}

response response::error(std::string_view message) {
    return response{response_type::ERROR, std::string{message}};
}

response response::number(uint64_t n) {
    return response{response_type::NUMBER, std::to_string(n)};
}

response response::logged_in(std::string_view message) {
    return response{response_type::LOGGED_IN, std::string{message}};
}

}
