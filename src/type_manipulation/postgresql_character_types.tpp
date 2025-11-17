//
// Created by fifthzoner on 16/11/2025.
//

#ifndef PPP_POSTGRESQL_CHARACTER_TYPES_TPP
#define PPP_POSTGRESQL_CHARACTER_TYPES_TPP

#include "postgresql_type_base.tpp"

namespace ppp::internal {

    template <typename I, bool is_fixed_length, bool is_blank_padded, bool has_max_length, bool is_blank_trimmed>
    struct _type_character : postgresql_type_base {
        I _value{};

        explicit _type_character(const std::size_t size) {
            if (int32_t(size) > 0)
                _value.reserve(size);
            else if constexpr (is_fixed_length)
                throw std::runtime_error("Length limited character type had non-positive size passed!");
        }
        ~_type_character() override = default;

        std::ostream& operator << (std::ostream& os) const override {
            return os << '\'' << _value << "\'";
        }

        std::istream& operator >> (std::istream& is) override {
            return is >> _value;
        }

        template <typename T>
        constexpr static bool is_allowed_cpp_type = std::is_convertible_v<I, T> or std::is_same_v<I, T>;

        template <typename T>
        constexpr static void throw_if_wrong_type() {
            if (not is_allowed_cpp_type<T>)
                throw std::runtime_error("Incompatible type passed to character value!");
        }


        template <typename T>
        _type_character& operator=(T&& value) {
            _value = I(value);
            return *this;
        }

        template <typename T>
        T as() {
            return T(_value);
        }
    };

    using postgresql_type_varchar_n = _type_character<std::string, false, false, true, false>;
    using postgresql_type_char = _type_character<std::string, true, true, false, false>;
    using postgresql_type_bpchar_n = _type_character<std::string, true, true, false, false>;
    using postgresql_type_bpchar = _type_character<std::string, false, true, false, true>;
    using postgresql_type_text = _type_character<std::string, false, false, false, false>;
}

#endif //PPP_POSTGRESQL_CHARACTER_TYPES_TPP