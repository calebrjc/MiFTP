#pragma once

#include <optional>
#include <string_view>
#include <vector>

namespace sftp {

/// @brief Represents the different types of requests that can be made to the server.
// clang-format off
enum class request_type {
    none = 0,

    user, acct, pass, type, list,
    cdir, kill, name, tobe, done,
    retr, send, stop, stor, size,
};
// clang-format on

/// @brief Represents a request made to the server.
struct request {
   public:
    /// @brief Return a request from the given raw bytes, or std::nullopt if no request could be
    /// parsed.
    /// @param raw_bytes The raw bytes to parse.
    /// @return A request from the given raw bytes, or std::nullopt if no request could be parsed.
    static std::optional<request> from_bytes(std::string_view raw_bytes);

   public:
    request_type type;
    std::vector<std::string> args;
};

}  // namespace sftp