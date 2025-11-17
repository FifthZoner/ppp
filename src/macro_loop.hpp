//
// Created by fifthzoner on 16/11/2025.
//

#ifndef PPP_MACRO_LOOP_HPP
#define PPP_MACRO_LOOP_HPP

#define INTERNAL_FOR_EACH_LOOP50(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) <!<>!ERROR LOOP LIMIT REACHED!<>!>
#define INTERNAL_FOR_EACH_LOOP49(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP50(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP48(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP49(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP47(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP48(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP46(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP47(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP45(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP46(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP44(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP45(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP43(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP44(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP42(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP43(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP41(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP42(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP40(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP41(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP39(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP40(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP38(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP39(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP37(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP38(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP36(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP37(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP35(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP36(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP34(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP35(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP33(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP34(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP32(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP33(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP31(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP32(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP30(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP31(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP29(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP30(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP28(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP29(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP27(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP28(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP26(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP27(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP25(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP26(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP24(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP25(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP23(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP24(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP22(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP23(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP21(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP22(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP20(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP21(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP19(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP20(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP18(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP19(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP17(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP18(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP16(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP17(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP15(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP16(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP14(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP15(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP13(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP14(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP12(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP13(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP11(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP12(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP10(E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP11(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP9( E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP10(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP8( E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP9( E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP7( E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP8( E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP6( E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP7( E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP5( E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP6( E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP4( E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP5( E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP3( E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP4( E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP2( E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP3( E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))
#define INTERNAL_FOR_EACH_LOOP1( E_less_macro, arg1, arg2, E_less_separator_macro, arg, ...) E##E_less_macro(arg, arg1, arg2) __VA_OPT__(E##E_less_separator_macro) __VA_OPT__(INTERNAL_FOR_EACH_LOOP2( E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))

#define FOR_EACH(E_less_macro, arg1, arg2, E_less_separator_macro, ...) __VA_OPT__(INTERNAL_FOR_EACH_LOOP1(E_less_macro, arg1, arg2, E_less_separator_macro, __VA_ARGS__))

#define EX_CASE(type, method_front, method_back) case ppp::internal::enum_##type: method_front type method_back ; break;

#define EX_COMMA ,
#define EX_COMMA_NEWLINE , \

#define EX_NEWLINE \

#include "type_manipulation/postgresql_type_base.tpp"

#define SWITCH_PSQL_TYPE_EXCEPTION_C(type_enum, method_before_type, method_after_type, invalid_message)         \
switch (type_enum) {                                                                                            \
    FOR_EACH(X_CASE, method_before_type, method_after_type, X_NEWLINE, PPP_REGISTERED_POSTGRESQL_TYPES)         \
default: throw std::runtime_error(invalid_message);                                                             \
}
#define SWITCH_PSQL_TYPE_EXCEPTION(type_enum, method_before_type, method_after_type)                            \
    SWITCH_PSQL_TYPE_EXCEPTION_C(type_enum, method_before_type, method_after_type, "Invalid dynamic type!")

#define SWITCH_PSQL_TYPE_E_MESSAGE_C(type_enum, method_before_type, method_after_type, invalid_message)         \
switch (type_enum) {                                                                                            \
    FOR_EACH(X_CASE, method_before_type, method_after_type, X_NEWLINE, PPP_REGISTERED_POSTGRESQL_TYPES)         \
default: std::cout << "TYPE ERROR: " << invalid_message; break;                                                 \
}
#define SWITCH_PSQL_TYPE_E_MESSAGE(type_enum, method_before_type, method_after_type)                            \
    SWITCH_PSQL_TYPE_EXCEPTION_C(type_enum, method_before_type, method_after_type, "Invalid dynamic type!\n")

#define EX_EXPAND_ENUM(type, ...) enum_##type

#define FOR_EACH_REGISTERED_TYPE_ENUM none, FOR_EACH(X_EXPAND_ENUM, , , X_COMMA_NEWLINE, PPP_REGISTERED_POSTGRESQL_TYPES)

#endif //PPP_MACRO_LOOP_HP