#include "sftp/request.hpp"

#include <algorithm>
#include <unordered_map>

namespace sftp {

namespace detail {

/// @brief Return a vector of strings from the given string, split by the given delimiter.
/// @param str The string to split.
/// @param delimiter The delimiter to split by.
/// @return A vector of strings from the given string, split by the given delimiter.
static std::vector<std::string> tokenize(std::string_view str, char delimiter) {
    std::vector<std::string> result;

    while (!str.empty()) {
        // Find the next delimiter
        auto delimiter_pos = str.find(delimiter);

        // If there are no more delimiters, push the remaining string and break
        if (delimiter_pos == std::string_view::npos) {
            result.push_back(std::string(str));
            break;
        }

        // Push the string up to the delimiter and remove it from the remaining string
        result.push_back(std::string(str.substr(0, delimiter_pos)));
        str = str.substr(delimiter_pos + 1);
    }

    return result;
}

/// @brief A map from ASCII request types to request types.
// clang-format off
static const std::unordered_map<std::string_view, request_type> ascii_to_request_type = {
    {"user", request_type::user}, {"acct", request_type::acct}, {"pass", request_type::pass},
    {"type", request_type::type}, {"list", request_type::list}, {"cdir", request_type::cdir},
    {"kill", request_type::kill}, {"name", request_type::name}, {"tobe", request_type::tobe},
    {"done", request_type::done}, {"retr", request_type::retr}, {"send", request_type::send},
    {"stop", request_type::stop}, {"stor", request_type::stor}, {"size", request_type::size},
};
// clang-format on

static request_type request_type_from_str(std::string_view ascii) {
    // Convert the ASCII to uppercase
    auto ascii_str = std::string(ascii);
    std::transform(ascii_str.begin(), ascii_str.end(), ascii_str.begin(), ::toupper);

    // Attempt to resolve the request type
    auto it = detail::ascii_to_request_type.find(ascii_str);

    // Return none if the request type couldn't be resolved
    if (it == detail::ascii_to_request_type.end()) return request_type::none;

    return it->second;
}

}  // namespace detail

std::optional<request> request::from_bytes(std::string_view raw_bytes) {
    // Return nullopt if there aren't enough bytes for a valid request
    if (raw_bytes.size() < 4) { return std::nullopt; }

    // Attempt to resolve the request type from the first 4 bytes
    auto type = detail::request_type_from_str(raw_bytes.substr(0, 4));
    if (type == request_type::none) { return std::nullopt; }

    // Split the remaining bytes by spaces into a vector of strings
    auto args = detail::tokenize(raw_bytes.substr(4), ' ');

    return request{type, args};
}

}  // namespace sftp
