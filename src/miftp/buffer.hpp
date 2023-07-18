#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <vector>

namespace calebrjc {

/// @brief A utility used to pass data around without ugly parameter passing.
struct Buffer {
    /// @brief Create a default buffer.
    Buffer() : data_() {}

    /// @brief Create a Buffer using existing data.
    ///
    /// @param data The data to be contained by the Buffer.
    /// @param size The number of bytes to be contained by the Buffer.
    Buffer(const char *data, size_t size) : data_(data, data + size) {}

    /// @brief Create a Buffer of a specific size with no meaningful data.
    ///
    /// @param size The number of bytes to be contained by the buffer.
    Buffer(size_t size) { data_.resize(size); }

    /// @brief Create a Buffer from an existing Buffer.
    ///
    /// @param other The Buffer to be copied from.
    Buffer(Buffer &other) : data_(other.data_) {}

    /// @brief Create a buffer from an existing POD array.
    ///
    /// @tparam T The type of the elements in the POD array.
    /// @tparam N The number of elements in the POD array.
    /// @param data The POD array to be contained by the Buffer.
    template <typename T, size_t N>
    Buffer(T (&data)[N]) : Buffer(data, sizeof(T) * (N - 1)) {}

    /// @brief Return the data contained by this Buffer.
    ///
    /// @return The data contained by this Buffer.
    char *data() { return data_.data(); }

    /// @brief Return the data contained by this Buffer.
    ///
    /// @return The data contained by this Buffer.
    const char *data() const { return data_.data(); }

    /// @brief Return the number of bytes contained by this Buffer.
    /// @return The number of bytes contained by this Buffer.
    size_t size() const { return data_.size(); }

  private:
    /// @brief The underlying storage for a Buffer.
    std::vector<char> data_;
};
}  // namespace calebrjc
