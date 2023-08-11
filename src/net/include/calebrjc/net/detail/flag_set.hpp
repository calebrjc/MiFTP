#pragma once

#include <bitset>
#include <cassert>
#include <iostream>
#include <string>

namespace calebrjc::net::detail {

/// @brief A type-safe way to use enums as flags, adapted from GitHub for my own needs. (ref:
/// https://github.com/mrts/flag-set-cpp)
template<typename T>
class FlagSet {
   public:
    FlagSet() = default;

    FlagSet(const T& val) { flags.set(static_cast<UnderlyingType>(val)); }

    FlagSet& operator&=(const T& val) noexcept {
        bool temp = flags.test(static_cast<UnderlyingType>(val));
        flags.reset();
        flags.set(static_cast<UnderlyingType>(val), temp);
        return *this;
    }

    FlagSet& operator|=(const T& val) noexcept {
        flags.set(static_cast<UnderlyingType>(val));
        return *this;
    }

    FlagSet operator&(const T& val) const {
        FlagSet result(*this);
        result &= val;

        return result;
    }

    FlagSet operator|(const T& val) const {
        FlagSet result(*this);
        result |= val;

        return result;
    }

    operator bool() const { return flags.any(); }

   private:
    using UnderlyingType = std::underlying_type_t<T>;

    // _ is last value sentinel and must be present in enum T.
    std::bitset<static_cast<UnderlyingType>(T::_)> flags;
};

template<typename T, typename = void>
struct IsEnumThatContainsSentinel : std::false_type {};

template<typename T>
struct IsEnumThatContainsSentinel<T, decltype(static_cast<void>(T::_))> : std::is_enum<T> {};

// Operator that combines two enumeration values into a FlagSet only if the
// enumeration contains the sentinel `_`.
template<typename T>
std::enable_if_t<IsEnumThatContainsSentinel<T>::value, FlagSet<T>> operator|(
    const T& lhs, const T& rhs) {
    FlagSet<T> fs;
    fs |= lhs;
    fs |= rhs;

    return fs;
}
}  // namespace calebrjc::net::detail
