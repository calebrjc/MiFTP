#pragma once

#include <optional>
#include <string_view>
#include <vector>

/// @brief Represents the different types of requests that can be made to the server.
enum class sftp_request_type {
    NONE = 0,
    USER,
    ACCT,
    PASS,
    TYPE,
    LIST,
    CDIR,
    KILL,
    NAME,
    TOBE,
    DONE,
    RETR,
    SEND,
    STOP,
    STOR,
    SIZE,
};

/// @brief Represents a request made to the server.
struct sftp_request {
   public:
    /// @brief Return a request from the given raw bytes, or std::nullopt if no request could be
    /// parsed.
    /// @param raw_bytes The raw bytes to parse.
    /// @return A request from the given raw bytes, or std::nullopt if no request could be parsed.
    static std::optional<sftp_request> from_bytes(std::string_view raw_bytes) {
        // TODO(Caleb): Implement this properly.
        return sftp_request{};
    }

   public:
    sftp_request_type type;
    std::vector<std::string> args;
};
