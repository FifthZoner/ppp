//
// Created by fifthzoner on 16/11/2025.
//

#ifndef PPP_INTERNAL_ROW_HPP
#define PPP_INTERNAL_ROW_HPP
#include <iostream>
#include <ostream>

#include "message.hpp"
#include "postgresql_types.hpp"

namespace ppp {
    class table;
}

namespace ppp::internal {

    struct typeless_value {
        void* _value = nullptr;
        const type_enum _type = none;

    private:

        template <postgresql_type T>
        void set_default_value(std::size_t size) {
            delete static_cast<T*>(_value);
            _value = new T(size);
        }

        template <postgresql_type T, usable_cpp_type V>
        void set_value(V&& value) {
            T::template throw_if_wrong_type<V>();
            if constexpr (T::template is_allowed_cpp_type<V>)
                static_cast<T*>(_value)->operator=(std::forward<T>(value));
        }

        template <postgresql_type T>
        void delete_value() {
            delete static_cast<T*>(_value);
        }

        template <postgresql_type T>
        constexpr T& as_type() {
            return *static_cast<T*>(_value);
        }

        template <postgresql_type T, usable_cpp_type V>
        constexpr V as(V*) {
            T::template throw_if_wrong_type<V>();
            if constexpr (T::template is_allowed_cpp_type<V>)
                return static_cast<T*>(_value)->template as<V>();
            return {};
        }

        template <postgresql_type T, usable_cpp_type V>
        constexpr void set(V&& value) {
            T::template throw_if_wrong_type<V>();
            if constexpr (T::template is_allowed_cpp_type<V>)
                static_cast<T*>(_value)->template operator=<V>(value);
        }

        template <postgresql_type T>
        constexpr void copy_value(const typeless_value& other) {
            _value = new T(*static_cast<T*>(other._value));
        }

    public:
        explicit typeless_value(type_enum type, std::size_t size) : _type(type) {
            SWITCH_PSQL_TYPE_EXCEPTION(type, set_default_value<, >(size));
        }

        typeless_value(typeless_value&& other)  noexcept : _type(other._type) {
            _value = other._value;
            other._value = nullptr;
        }

        typeless_value(const typeless_value& other) : _type(other._type) {
            SWITCH_PSQL_TYPE_EXCEPTION(_type, copy_value<, >(other));
        }

        ~typeless_value() {
            if (_value != nullptr)
                try {
                    SWITCH_PSQL_TYPE_EXCEPTION(_type, delete_value<, >());
                }
                catch (...) {
                    std::cout << "TYPE ERROR: failed destroy a typed object!\n";
                }
        }

        template <usable_cpp_type V>
        V operator()() {
            SWITCH_PSQL_TYPE_EXCEPTION(_type, return as<, >(static_cast<V*>(nullptr)));
        }

        template <usable_cpp_type V>
        V& operator()(V& place_to_set) {
            SWITCH_PSQL_TYPE_EXCEPTION(_type, return place_to_set = as<, >(static_cast<V*>(nullptr)));
        }

        template <usable_cpp_type V>
        typeless_value& operator=(V&& value) {
            if constexpr (std::same_as<V, std::string> or std::same_as<V, std::string&> or std::same_as<V, std::string&&>) {
                std::istringstream s{value};
                SWITCH_PSQL_TYPE_EXCEPTION(_type, as_type<, >().operator>>(s));
            }
            else
                SWITCH_PSQL_TYPE_EXCEPTION(_type, set<, >(std::forward<V>(value)));
            return *this;
        }

        std::ostream& operator << (std::ostream& os) {
            SWITCH_PSQL_TYPE_EXCEPTION(_type, return as_type<, >().operator<<(os));
        }

        std::istream& operator >> (std::istream& is) {
            SWITCH_PSQL_TYPE_EXCEPTION(_type, return as_type<, >().operator>>(is));
        }
    };

    struct internal_row {
        std::vector<typeless_value> _data{};

        void set_data_from_table(const table& table);

        explicit internal_row(const table& table);
        internal_row(const table& table, message&& message);
        internal_row(const internal_row& other);
        internal_row(internal_row&& other) noexcept;

        typeless_value operator[](const table& table, const std::string& name);
        typeless_value operator[](const table& table, std::size_t index);
    };

    std::ostream& operator<< (std::ostream& os, internal_row& row);
}

#endif //PPP_INTERNAL_ROW_HPP