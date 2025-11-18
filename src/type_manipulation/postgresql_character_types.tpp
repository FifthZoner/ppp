//
// Created by fifthzoner on 16/11/2025.
//

#ifndef PPP_POSTGRESQL_CHARACTER_TYPES_TPP
#define PPP_POSTGRESQL_CHARACTER_TYPES_TPP

#include "postgresql_type_base.tpp"

namespace ppp::internal {

    template<bool> struct optional_type_size;
    template<> struct optional_type_size<false> {};
    template<> struct optional_type_size<true> { std::size_t size = 0; };

    template <bool is_fixed_length_and_padded, bool has_max_length, bool is_blank_trimmed>
    struct _type_character : postgresql_type_base {
        std::string _value{};
        optional_type_size<has_max_length> size = {};

        explicit _type_character(const std::size_t size) {
            if (int32_t(size) > 0)
                _value.reserve(size);
            else if constexpr (is_fixed_length_and_padded)
                throw std::runtime_error("Length limited character type had non-positive size passed!");
        }
        ~_type_character() override = default;

        std::ostream& operator << (std::ostream& os) const override {
            return os << '\'' << _value << "\'";
        }

        template <typename T>
        constexpr static bool is_allowed_cpp_type = std::is_convertible_v<std::string, T>
                                                 or std::is_same_v<std::string, T>
                                                 or std::is_arithmetic_v<T>
        ;

        template <typename T>
        constexpr static void throw_if_wrong_type() {
            if (not is_allowed_cpp_type<T>)
                throw std::runtime_error("Incompatible type passed to character value!");
        }


        template <typename T>
        _type_character& operator=(T&& value) {
            std::string parsed{};

            if constexpr (std::convertible_to<T, std::string>)
                parsed = std::string(value);
            else
                parsed = std::to_string(value);

            if constexpr (is_fixed_length_and_padded) {
                if (parsed.size() > _value.capacity())
                    throw std::runtime_error("Provided value exceeds field's size after any conversions!");


            }

            // TODO: add the rest of checks

            _value = std::move(parsed);

            return *this;
        }

        std::istream& operator >> (std::istream& is) override {
            std::string read{};
            is >> read;
            this->operator=<std::string>(std::move(read));
            return is;
        }

        template <typename T>
        T as() {
            if constexpr (std::convertible_to<std::string, T>)
                return T(_value);
            else {
                if (_value.contains('.') or _value.contains(','))
                    return T(std::stold(_value));
                if (_value.contains('-'))
                    return T(std::stoll(_value));
                return T(std::stoull(_value));
            }
        }
    };

    using postgresql_type_varchar_n = _type_character<false, true, false>;
    using postgresql_type_char = _type_character<true, false, false>;
    using postgresql_type_bpchar_n = _type_character<true, false, false>;
    using postgresql_type_bpchar = _type_character<false, false, true>;
    using postgresql_type_text = _type_character<false, false, false>;
}

#endif //PPP_POSTGRESQL_CHARACTER_TYPES_TPP