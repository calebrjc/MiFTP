#pragma once

#include <string>

namespace sftp {

/// @brief Represents the different types of responses that can be sent to the client.
enum class response_type {
    NONE = 0,
    SUCCESS,
    ERROR,
    NUMBER,
    LOGGED_IN,
};

/// @brief Represents a response sent to the client.
struct response {
   public:
    /// @brief Return a response that indicates success, with the given message.
    /// @param message The message to send along with the response.
    /// @return A response that indicates success, with the given message.
    static response success(std::string_view message);

    /// @brief Return a response that indicates failure, with the given message.
    /// @param message The message to send along with the response.
    /// @return A response that indicates failure, with the given message.
    static response error(std::string_view message);

    /// @brief Return a response that indicates a number, with the given number.
    /// @param n The number to send along with the response.
    /// @return A response that indicates a number, with the given number.
    static response number(uint64_t n);

    /// @brief Return a response that indicates a successful login, with the given message.
    /// @param message The message to send along with the response.
    /// @return A response that indicates a successful login, with the given message.
    static response logged_in(std::string_view message);

   public:
    response_type type;
    std::string message;
};

}  // namespace sftp
