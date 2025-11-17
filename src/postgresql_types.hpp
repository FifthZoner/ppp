//
// Created by fifthzoner on 16/11/2025.
//
#ifndef PPP_POSTGRESQL_TYPES_HPP
#define PPP_POSTGRESQL_TYPES_HPP

#include <concepts>
#include <type_traits>
#include <unordered_map>

#include "type_manipulation/postgresql_type_base.tpp"
#include "type_manipulation/postgresql_numeric_types.tpp"
#include "type_manipulation/postgresql_character_types.tpp"
#include "macro_loop.hpp"

namespace ppp::internal {

    enum type_enum : uint8_t {
        FOR_EACH_REGISTERED_TYPE_ENUM
    };

    struct test_type {};

    template<typename T>
    concept postgresql_type_has_method_as = requires(T t) {
        { t.template as<test_type>() } -> std::same_as<test_type>;
    };

    template<typename T>
    concept postgresql_type_has_method_assign = requires(T t) {
        { t.template operator=<test_type>(test_type()) } -> std::same_as<T&>;
    };

    template<typename T>
    concept postgresql_type_has_method_ostream = requires(T t, std::ostream& os) {
        { t.operator<<(os) } -> std::same_as<std::ostream&>;
    };

    template<typename T>
    concept postgresql_type_has_method_istream = requires(T t, std::istream& is) {
        { t.operator>>(is) } -> std::same_as<std::istream&>;
    };

    template<typename T>
    concept postgresql_type_has_method_constructor = requires {
        { T(std::size_t()) } -> std::same_as<T>;
    };

    template<typename T>
    concept postgresql_type_has_method_destructor = requires(T t) {
        { t.~T() } -> std::same_as<void>;
    };

    template<typename T>
    concept postgresql_type_has_bool_cpp_type_allower = requires {
        { T::template is_allowed_cpp_type<int> };
    };

    template<typename T>
    concept postgresql_type_has_throw_if_wrong_type = requires {
        { T::template throw_if_wrong_type<int>() } -> std::same_as<void>;
    };

    template <typename T, typename... Rs>
    constexpr bool is_one_of_types = (std::is_same_v<T, Rs> or ...);

    template <typename T>
    constexpr bool is_registered_postgresql_type = is_one_of_types<T, PPP_REGISTERED_POSTGRESQL_TYPES>;

    template<typename T>
    concept postgresql_type = std::is_base_of_v<postgresql_type_base, T>
                          and postgresql_type_has_method_as<T>
                          and postgresql_type_has_method_assign<T>
                          and postgresql_type_has_method_ostream<T>
                          and postgresql_type_has_method_istream<T>
                          and postgresql_type_has_method_constructor<T>
                          and postgresql_type_has_method_destructor<T>
                          and postgresql_type_has_bool_cpp_type_allower<T>
                          and postgresql_type_has_throw_if_wrong_type<T>
                          and is_registered_postgresql_type<T>;

    template<typename T, typename... Rs>
    concept _usable_cpp_type = (Rs::template is_allowed_cpp_type<T> or ...);

    template <typename T>
    constexpr bool _usable_cpp_type_bool = _usable_cpp_type<T, PPP_REGISTERED_POSTGRESQL_TYPES>;

    template<typename T, typename... Rs>
        concept usable_cpp_type = requires {
        _usable_cpp_type_bool<T>;
    };


    // just to make sure the conditions work correctly
    template <postgresql_type T>
       struct test {};

    using test2 = test<postgresql_type_real>;

    inline std::unordered_map<std::string, type_enum> _type_name_map = {
        {"int2", enum_postgresql_type_integer_2},
        {"int4", enum_postgresql_type_integer_4},
        {"int8", enum_postgresql_type_integer_8},
        {"char", enum_postgresql_type_char},
        {"varchar", enum_postgresql_type_varchar_n},
        {"bpchar", enum_postgresql_type_bpchar_n}
    };

    inline type_enum get_type_enum_by_name(std::string name) {
        if (_type_name_map.contains(name))
            return _type_name_map[name];
        return none;
    }


}

#endif //PPP_POSTGRESQL_TYPES_HPP