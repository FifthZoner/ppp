//
// Created by fifthzoner on 16/11/2025.
//

#ifndef PPP_POSTGRESQL_TYPE_BASE_TPP
#define PPP_POSTGRESQL_TYPE_BASE_TPP

#include <ostream>
#include <istream>

namespace ppp::internal {

    // type registration
#define PPP_REGISTERED_POSTGRESQL_NUMERIC_TYPES \
    postgresql_type_integer_2,\
    postgresql_type_integer_4,\
    postgresql_type_integer_8,\
    postgresql_type_real,     \
    postgresql_type_double,   \
    postgresql_type_serial_2, \
    postgresql_type_serial_4, \
    postgresql_type_serial_8

#define PPP_REGISTERED_POSTGRESQL_CHARACTER_TYPES \
    postgresql_type_varchar_n, \
    postgresql_type_char,      \
    postgresql_type_bpchar_n,  \
    postgresql_type_bpchar,    \
    postgresql_type_text

#define PPP_REGISTERED_POSTGRESQL_TYPES \
PPP_REGISTERED_POSTGRESQL_NUMERIC_TYPES, \
PPP_REGISTERED_POSTGRESQL_CHARACTER_TYPES


    // the base type for all postgresql types
    struct postgresql_type_base {

        // required methods, implemented without actual functionality here

        // cannot be instantiated
        postgresql_type_base() {
            throw_if_wrong_type<postgresql_type_base>();
        }

        virtual ~postgresql_type_base() {
            throw_if_wrong_type<postgresql_type_base>();
        }

        // for output and input in queries
        virtual std::ostream& operator << (std::ostream& os) const {
            return os;
        }

        virtual std::istream& operator >> (std::istream& is) {
            return is;
        }

        template <typename T>
        constexpr static bool is_allowed_cpp_type = false;

        // this must be called at runtime since it needs to compile when we don't know the type
        // and only throw if we get here
        // also it does not need to be defined for every derived type
        template <typename T>
        constexpr static void throw_if_wrong_type() {
            if (not is_allowed_cpp_type<T>)
                throw std::runtime_error("Incompatible type passed to row value!");
        }

        template <typename T>
        postgresql_type_base& operator=(T&& value) {
            // substitute with assigning the value of T to the actual type
            T discarded = value;
            return *this;
        }

        template <typename T>
        T as() {
            // substitute with creating instance of T from the actual type
            return T{};
        }
    };
}

#endif //PPP_POSTGRESQL_TYPE_BASE_TPP