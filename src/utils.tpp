//
// Created by fifthzoner on 11/11/2025.
//

#ifndef PPP_UTILS_TPP
#define PPP_UTILS_TPP
#include <cstdint>
#include <type_traits>

namespace ppp::internal {
    /// Reverses a bytes of a numeric value and returns the reverted value
    /// @tparam T type of value, must be numeric
    /// @param value the value
    /// @return byte reversed value
    template <typename T>
    requires std::is_arithmetic_v<T>
    T reverse(T value) {
        for (std::size_t n = 0; n < sizeof(T) / 2; n++)
            std::swap(((uint8_t*)&value)[n], ((uint8_t*)&value)[sizeof(T) - 1 - n]);
        return value;
    }

    /// Creates a value of given numeric types from a pointer to it's first reverted byte in a buffer
    /// @tparam T type of constructed value
    /// @param value pointer to value
    /// @return re-reverted value constructed from the pointer
    template <typename T>
    requires std::is_arithmetic_v<T>
    T reverse(const void* value) {
        T result{};
        for (std::size_t n = 0; n < sizeof(T); n++)
            ((uint8_t*)&result)[n] = ((uint8_t*)value)[sizeof(T) - 1 - n];
        return result;
    }

    //template <is_postgresql_type T>
    //bool is_type_compatible(field_def field) {
    //    if (field.)
    //}
}

#endif //PPP_UTILS_TPP