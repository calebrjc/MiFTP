#include "sftp/request.hpp"

std::optional<sftp_request> sftp_request::from_bytes(std::string_view raw_bytes) {
    (void)raw_bytes;
    return std::nullopt;
}
