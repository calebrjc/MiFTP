#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

namespace calebrjc::net {
/// @brief A utility used to wrap normal buffer operations for convenience.
struct buffer {
    /// @brief Create an empty buffer.
    buffer() {}

    /// @brief Create a buffer using existing data.
    /// @param data The data to be contained by the buffer.
    /// @param size The number of bytes to be contained by the buffer.
    buffer(const char *data, size_t size) : data_(data, data + size) {}

    /// @brief Create a buffer of a specific size with no meaningful data.
    /// @param size The number of bytes to be contained by the buffer.
    buffer(size_t size) { data_.resize(size); }

    /// @brief Create a buffer from an existing POD array.
    /// @tparam T The type of the elements in the POD array.
    /// @tparam N The number of elements in the POD array.
    /// @param data The POD array to be contained by the buffer.
    template<typename T, size_t N>
    buffer(T (&data)[N]) : buffer(data, sizeof(T) * (N - 1)) {}

    /// @brief Return the data contained by this buffer.
    /// @return The data contained by this buffer.
    char *data() { return data_.data(); }

    /// @brief Return the data contained by this buffer.
    /// @return The data contained by this buffer.
    const char *data() const { return data_.data(); }

    /// @brief Return the number of bytes contained by this buffer.
    /// @return The number of bytes contained by this buffer.
    size_t size() const { return data_.size(); }

    /// @brief Zero out the underlying memory contained by this buffer.
    void zero() { std::memset(data_.data(), 0, data_.size()); }

    /// @brief Return true if this buffer contains any data, and false otherwise.
    operator bool() const { return data_.size() > 0; }

    /// @brief Return the data contained by this buffer in string form.
    /// @return The data contained by this buffer in string form.
    std::string str() const { return std::string(data_.data(), data_.size()); }

    /// @brief Return true if this buffer contains the same data as the other buffer.
    /// @param other The other buffer in this comparison.
    /// @return True if this buffer contains the same data as the other buffer.
    bool operator==(const buffer &other) const {
        if (size() != other.size()) return false;

        return std::memcmp(data(), other.data(), size());
    }

   private:
    /// @brief The underlying storage for this buffer.
    std::vector<char> data_;
};
}  // namespace calebrjc::net
