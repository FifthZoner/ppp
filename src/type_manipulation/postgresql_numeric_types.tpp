//
// Created by fifthzoner on 16/11/2025.
//

#ifndef PPP_POSTGRESQL_NUMERIC_TYPES_TPP
#define PPP_POSTGRESQL_NUMERIC_TYPES_TPP

#include "postgresql_type_base.tpp"

namespace ppp::internal {

    template <typename I>
    struct _type_integer : postgresql_type_base {
        I _value = 0;

        explicit _type_integer(std::size_t size) {
            if (size != sizeof(I))
                throw std::runtime_error("Invalid integer type size constructor argument!");
        };
        ~_type_integer() override = default;

        std::ostream& operator << (std::ostream& os) const override {
            return os << _value;
        }

        std::istream& operator >> (std::istream& is) override {
            return is >> _value;
        }

        template <typename T>
        constexpr static bool is_allowed_cpp_type = std::is_convertible_v<I, T>;

        template <typename T>
        _type_integer& operator=(T&& value) {
            _value = I(value);
            return *this;
        }

        template <typename T>
        T as() {
            return T(_value);
        }
    };

    using postgresql_type_integer_2 = _type_integer<int16_t>;
    using postgresql_type_integer_4 = _type_integer<int32_t>;
    using postgresql_type_integer_8 = _type_integer<int64_t>;
    using postgresql_type_real      = _type_integer<float>;
    using postgresql_type_double    = _type_integer<double>;
    using postgresql_type_serial_2  = _type_integer<uint16_t>;
    using postgresql_type_serial_4  = _type_integer<uint32_t>;
    using postgresql_type_serial_8  = _type_integer<uint64_t>;
}

#endif //PPP_POSTGRESQL_NUMERIC_TYPES_TPP