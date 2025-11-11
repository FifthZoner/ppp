//
// Created by fifthzoner on 11/11/2025.
//

#include <asio.hpp>

#include "message.hpp"

#include <iostream>

namespace ppp::internal {

    message message::create_startup_message(std::string&& user_name) {
        message msg{};
        msg.type = startup_message;
        msg.data.pop_back();
        msg.add(0x00030000);
        msg.add("user");
        msg.add(std::move(user_name));
        msg.add('\0');
        msg.finalize();
        return msg;
    }

    message message::create_sasl_initial_response(std::string&& auth_method, std::vector<uint8_t>&& initial_response) {
        message msg{};
        msg.type = sasl_initial_response;

        // String - name of auth mechanism
        msg.add(std::move(auth_method));

        // int32 - length of response, -1 if none
        int32_t response_length = initial_response.size();
        if (response_length == 0)
            response_length = -1;

        msg.add(response_length);

        // byte[n] - initial response
        if (response_length != -1)
            msg.add(initial_response.data(), response_length);

        msg.finalize();
        return msg;
    }

    message message::create_sasl_response(std::vector<uint8_t>&& response) {
        message msg{};
        msg.type = sasl_response;

        msg.add(response.data(), response.size());

        msg.finalize();
        return msg;
    }

    void parse_authentication_message(message& msg) {
        const uint8_t* ptr = msg.data.data() + 1;
        if (msg.data.size() < 9) {
            std::cout << "Invalid auth message length: " << msg.data.size() << "\n";
            return;
        }

        auto len = reverse<uint32_t>(ptr);

        if (len < 8 or len > msg.data.size() - 1) {
            std::cout << "Invalid provided auth message length: " << len << "\n";
            return;
        }

        auto type = reverse<uint32_t>(ptr += 4);

        switch (type) {
        case 0:
            msg.type = authentication_ok;
            return;
        case 2:
            msg.type = authentication_kerberos_v5;
            return;
        case 3:
            msg.type = authentication_cleartext_password;
            return;
        case 5:
            msg.type = authentication_md5_password;
            break;
        case 7:
            msg.type = authentication_gss;
            return;
        case 8:
            msg.type = authentication_gss_continue;
            break;
        case 9:
            msg.type = authentication_sspi;
            return;
        case 10:
            msg.type = authentication_sasl;
            return;
        case 11:
            msg.type = authentication_sasl_continue;
            return;
        case 12:
            msg.type = authentication_sasl_final;
            return;
        default:
            std::cout << "Invalid authentication message type integer value: " << type << "\n";
            return;
        }

        std::cout << "Unimplemented auth method!\n";
    }

    message message::create_from_data(std::vector<uint8_t>&& buffer) {
        message msg{};
        msg.data = std::move(buffer);

        if (msg.data.empty())
            return msg;

        switch (msg.data[0]) {
        case 'R':
            parse_authentication_message(msg);
            break;
        default:
            break;
        }

        return msg;
    }

    uint8_t get_frontend_message_first_byte(message_type type) {
        switch (type) {
        case sasl_initial_response:
        case sasl_response:
            return 'p';
        default:
            return '\0';
        }
    }

    bool does_have_front_byte(message_type type) {
        switch (type) {
        case cancel_request:
        case gssenc_request:
        case ssl_request:
        case startup_message:
        case unknown:
            return false;
        default:
            return true;
        }
    }

    void message::finalize() {
        uint32_t size = data.size() - 1;

        // first message byte
        auto* ptr = data.data();
        if (does_have_front_byte(type))
            *(ptr++) = get_frontend_message_first_byte(type);
        else size++;

        // after the usual 1'st byte with message type there's a 4 byte uint with size
        size = reverse(size);
        memcpy(ptr, &size, sizeof(size));

        // after that there's the data entered earlier so we don't care about that
    }

    std::string string_from_bytes(const uint8_t* bytes, std::size_t length) {
        std::string value{};
        value.resize(length);
        memcpy(value.data(), bytes, length);
        return value;
    }

    std::string message::authentication_sasl_get_method_string() const {
        return string_from_bytes(data.data() + 9, data.size() - 11);
    }

    std::string message::authentication_sasl_continue_get_challenge() const {
        return string_from_bytes(data.data() + 9, data.size() - 9);
    }

    std::string message::authentication_sasl_final_get_additional_data() const {
        return string_from_bytes(data.data() + 9, data.size() - 9);
    }
}
