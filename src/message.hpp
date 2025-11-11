//
// Created by fifthzoner on 11/11/2025.
//

#ifndef PPP_MESSAGE_HPP
#define PPP_MESSAGE_HPP

#include <cstdint>
#include <vector>
#include <asio.hpp>
#include "utils.tpp"

namespace ppp::internal {

    enum message_type : uint8_t {
        authentication_ok,
        authentication_kerberos_v5,
        authentication_cleartext_password,
        authentication_md5_password,
        authentication_gss,
        authentication_gss_continue,
        authentication_sspi,
        authentication_sasl,
        authentication_sasl_continue,
        authentication_sasl_final,
        backend_key_data,
        bind,
        bind_complete,
        cancel_request,
        close,
        close_complete,
        command_complete,
        copy_data,
        copy_done,
        copy_fail,
        copy_in_response,
        copy_out_response,
        copy_both_response,
        data_row,
        describe,
        empty_query_response,
        error_response,
        execute,
        flush,
        function_call,
        function_call_response,
        gssenc_request,
        gss_response,
        negotiate_protocol_version,
        no_data,
        notice_response,
        notification_response,
        parameter_description,
        parameter_status,
        parse,
        parse_complete,
        password_message,
        portal_suspended,
        query,
        ready_for_query,
        row_description,
        sasl_initial_response,
        sasl_response,
        ssl_request,
        startup_message,
        sync,
        terminate,
        unknown
    };

    struct message {

        std::vector<uint8_t> data = { 0, 0, 0, 0, 0};
        message_type type = unknown;
        uint32_t _helper = 0;

        inline void _add(const void* src_ptr, std::size_t count) {
            auto start = data.size();
            // <data> + <string> + \0
            data.resize(data.size() + count);
            auto* dest_ptr = data.data() + start;
            memcpy(dest_ptr, src_ptr, count);
        }

        template <typename T>
            requires std::is_arithmetic_v<T>
        void add(T value) {
            value = internal::reverse(value);
            _add(&value, sizeof(T));
        }

        template <typename T>
            requires std::is_arithmetic_v<T>
        void add(T* values, std::size_t count) {
            for (std::size_t n = 0; n < count; n++) {
                T value = internal::reverse(values[n]);
                _add(&value, sizeof(T));
            }
        }

        inline void add(std::string&& value) {
            _add(value.c_str(), value.size() + 1);
        }

        static message create_startup_message(std::string&& user_name, std::string&& database = {});
        static message create_sasl_initial_response(std::string&& auth_method, std::vector<uint8_t>&& initial_response);
        static message create_sasl_response(std::vector<uint8_t>&& response);
        static message create_query_message(std::string&& text);
        static message create_terminate_message();

        static std::vector<message> create_from_data(std::vector<uint8_t>&& buffer);

        void finalize();

        // getters for data about given message types
        std::string authentication_sasl_get_method_string() const;
        std::string authentication_sasl_continue_get_challenge() const;
        std::string authentication_sasl_final_get_additional_data() const;

    };

}


#endif //PPP_MESSAGE_HPP