#pragma once

#include <string>

/// @brief Represents the different types of responses that can be sent to the client.
enum class sftp_response_type {
    NONE = 0,
    SUCCESS,
    ERROR,
    NUMBER,
    LOGGED_IN,
};

/// @brief Represents a response sent to the client.
struct sftp_response {
   public:
    /// @brief Return a response that indicates success, with the given message.
    /// @param message The message to send along with the response.
    /// @return A response that indicates success, with the given message.
    static sftp_response success(std::string_view message) {
        return sftp_response{sftp_response_type::SUCCESS, std::string{message}};
    }

    /// @brief Return a response that indicates failure, with the given message.
    /// @param message The message to send along with the response.
    /// @return A response that indicates failure, with the given message.
    static sftp_response error(std::string_view message) {
        return sftp_response{sftp_response_type::ERROR, std::string{message}};
    }

    /// @brief Return a response that indicates a number, with the given number.
    /// @param n The number to send along with the response.
    /// @return A response that indicates a number, with the given number.
    static sftp_response number(uint64_t n) {
        return sftp_response{sftp_response_type::NUMBER, std::to_string(n)};
    }

    /// @brief Return a response that indicates a successful login, with the given message.
    /// @param message The message to send along with the response.
    /// @return A response that indicates a successful login, with the given message.
    static sftp_response logged_in(std::string_view message) {
        return sftp_response{sftp_response_type::LOGGED_IN, std::string{message}};
    }

   public:
    sftp_response_type type;
    std::string message;
};
