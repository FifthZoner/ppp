//
// Created by fifthzoner on 11/11/2025.
//

#include <asio.hpp>

#include "message.hpp"

#include <iostream>

namespace ppp::internal {

    message message::create_startup_message(std::string&& user_name, std::string&& database) {
        message msg{};
        std::cout << "MESSAGE: creating startup message\n";
        msg.type = startup_message;
        msg.data.pop_back();
        std::cout << "    INFO: protocol version: 3.0\n";
        msg.add(0x00030000);
        std::cout << "    INFO: user: " << user_name << "\n";
        msg.add("user");
        msg.add(std::move(user_name));
        if (not database.empty()) {
            std::cout << "    INFO: database: " << database << "\n";
            msg.add("database");
            msg.add(std::move(database));
        }
        msg.add('\0');
        msg.finalize();
        return msg;
    }

    message message::create_sasl_initial_response(std::string&& auth_method, std::vector<uint8_t>&& initial_response) {
        message msg{};
        std::cout << "MESSAGE: creating SASL/other initial response message\n";
        msg.type = sasl_initial_response;

        // String - name of auth mechanism
        std::cout << "    INFO: method: " << auth_method << "\n";
        msg.add(std::move(auth_method));

        // int32 - length of response, -1 if none
        int32_t response_length = initial_response.size();
        if (response_length == 0)
            response_length = -1;

        msg.add(response_length);

        // byte[n] - initial response
        if (response_length != -1) {
            std::cout << "    INFO: response content: " << reinterpret_cast<const char*>(initial_response.data()) << "\n";
            msg.add(initial_response.data(), response_length);
        }

        msg.finalize();
        return msg;
    }

    message message::create_sasl_response(std::vector<uint8_t>&& response) {
        message msg{};
        std::cout << "MESSAGE: creating SASL response message\n";
        msg.type = sasl_response;

        std::cout << "    INFO: response content: " << reinterpret_cast<const char*>(response.data()) << "\n";
        msg.add(response.data(), response.size());

        msg.finalize();
        return msg;
    }

    message message::create_query_message(std::string&& text) {
        message msg{};
        std::cout << "MESSAGE: creating query message\n";
        msg.type = query;
        std::cout << "    INFO: query content: " << text << "\n";
        msg.add(std::move(text));
        msg.finalize();
        return msg;
    }

    message message::create_terminate_message() {
        message msg{};
        std::cout << "MESSAGE: creating termination message\n";
        msg.type = terminate;
        msg.finalize();
        return msg;
    }

    message parse_authentication_message(const std::vector<uint8_t>& buffer, uint32_t& offset) {
        message msg{};
        std::cout << "MESSAGE: authentication request\n";

        const uint8_t* ptr = buffer.data() + 1 + offset;
        if (buffer.size() - offset < 9) {
            std::cout << "ERROR: Invalid auth message length: " << buffer.size() << "\n";
            return msg;
        }

        auto len = reverse<uint32_t>(ptr);

        if (len < 8 or len > buffer.size() - 1 - offset) {
            std::cout << "ERROR: Invalid provided auth message length: " << len << "\n";
            return msg;
        }

        offset += len + 1;
        msg.data.resize(len + 1);
        memcpy(msg.data.data(), ptr - 1, len + 1);

        auto type = reverse<uint32_t>(ptr += 4);

        switch (type) {
        case 0:
            msg.type = authentication_ok;
            std::cout << "    INFO: auth message type: ok\n";
            return msg;
        case 2:
            msg.type = authentication_kerberos_v5;
            std::cout << "    INFO: auth message type: kerberos v5\n";
            return msg;
        case 3:
            msg.type = authentication_cleartext_password;
            std::cout << "    INFO: auth message type: cleartext password\n";
            return msg;
        case 5:
            msg.type = authentication_md5_password;
            std::cout << "    INFO: md5 password\n";
            break;
        case 7:
            msg.type = authentication_gss;
            std::cout << "    INFO: auth message type: gss\n";
            return msg;
        case 8:
            msg.type = authentication_gss_continue;
            std::cout << "    INFO: auth message type: gss continue\n";
            break;
        case 9:
            msg.type = authentication_sspi;
            std::cout << "    INFO: auth message type: sspi\n";
            return msg;
        case 10:
            msg.type = authentication_sasl;
            std::cout << "    INFO: auth message type: sasl\n";
            return msg;
        case 11:
            msg.type = authentication_sasl_continue;
            std::cout << "    INFO: auth message type: sasl continue\n";
            std::cout << "    INFO: challenge value: " << msg.authentication_sasl_continue_get_challenge() << "\n";
            return msg;
        case 12:
            msg.type = authentication_sasl_final;
            std::cout << "    INFO: auth message type: sasl final\n";
            std::cout << "    INFO: additional info: " << msg.authentication_sasl_final_get_additional_data() << "\n";
            return msg;
        default:
            std::cout << "ERROR: Invalid authentication message type integer value: " << type << "\n";
            return msg;
        }

        std::cout << "ERROR: Unimplemented auth method!\n";
        msg.type = unknown;

        return msg;
    }

    message parse_parameter_status_message(const std::vector<uint8_t>& buffer, uint32_t& offset) {
        message msg{};
        std::cout << "MESSAGE: parameter status\n";


        const uint8_t* ptr = buffer.data() + 1 + offset;
        if (buffer.size() - offset < 7) {
            std::cout << "ERROR: Invalid message length: " << buffer.size() << "\n";
            return msg;
        }

        auto len = reverse<uint32_t>(ptr);

        if (len < 6 or len > buffer.size() - 1 - offset) {
            std::cout << "ERROR: Invalid provided message length: " << len << "\n";
            return msg;
        }

        offset += len + 1;
        msg.data.resize(len + 1);
        memcpy(msg.data.data(), ptr - 1, len + 1);

        ptr += 4;
        auto name_len = strnlen(reinterpret_cast<const char*>(ptr), len - 4);
        if (name_len == len - 4) {
            std::cout << "Cound not find name string null terminator!\n";
            return msg;
        }
        msg._helper = name_len;
        std::cout << "    INFO: parameter name: " << (char*)ptr << "\n";

        if (*(ptr + name_len) != '\0') {
            std::cout << "Cound not find value string null terminator!\n";
            return msg;
        }

        std::cout << "    INFO: parameter value: " << (char*)(ptr + name_len + 1) << "\n";

        msg.type = parameter_status;
        return msg;
    }

    message parse_backend_key_data_message(const std::vector<uint8_t>& buffer, uint32_t& offset) {
        message msg{};
        std::cout << "MESSAGE: backend key data\n";


        const uint8_t* ptr = buffer.data() + 1 + offset;
        if (buffer.size() - offset < 13) {
            std::cout << "ERROR: Invalid message length: " << buffer.size() << "\n";
            return msg;
        }

        auto len = reverse<uint32_t>(ptr);

        if (len != 12) {
            std::cout << "ERROR: Invalid provided message length: " << len << "\n";
            return msg;
        }

        offset += len + 1;
        msg.data.resize(len + 1);
        memcpy(msg.data.data(), ptr - 1, len + 1);

        std::cout << "    INFO: backend process id: " << reverse<uint32_t>(ptr + 4) << "\n";
        std::cout << "    INFO: backend secret key: " << reverse<uint32_t>(ptr + 8) << "\n";

        msg.type = parameter_status;
        return msg;
    }

    message parse_ready_for_query_message(const std::vector<uint8_t>& buffer, uint32_t& offset) {
        message msg{};
        std::cout << "MESSAGE: ready for query\n";


        const uint8_t* ptr = buffer.data() + 1 + offset;
        if (buffer.size() - offset < 6) {
            std::cout << "ERROR: Invalid message length: " << buffer.size() << "\n";
            return msg;
        }

        auto len = reverse<uint32_t>(ptr);

        if (len != 5) {
            std::cout << "ERROR: Invalid provided message length: " << len << "\n";
            return msg;
        }

        offset += len + 1;
        msg.data.resize(len + 1);
        memcpy(msg.data.data(), ptr - 1, len + 1);

        std::cout << "    INFO: ready for query status byte: " <<  (char)*(ptr + 4) << "\n";

        msg.type = ready_for_query;
        return msg;
    }

    std::string string_from_bytes(const uint8_t* bytes, std::size_t length) {
        std::string value{};
        value.resize(length);
        memcpy(value.data(), bytes, length);
        return value;
    }

    message parse_row_description_message(const std::vector<uint8_t>& buffer, uint32_t& offset) {
        message msg{};
        std::cout << "MESSAGE: row description\n";


        const uint8_t* ptr = buffer.data() + 1 + offset;
        if (buffer.size() - offset < 6) {
            std::cout << "ERROR: Invalid message length: " << buffer.size() << "\n";
            return msg;
        }

        auto len = reverse<uint32_t>(ptr);

        if (len > buffer.size() - offset) {
            std::cout << "ERROR: Invalid provided message length: " << len << "\n";
            return msg;
        }

        offset += len + 1;
        msg.data.resize(len + 1);
        memcpy(msg.data.data(), ptr - 1, len + 1);

        std::cout << "    INFO: unparsed for now, part of raw data: " << string_from_bytes(ptr + 4, len - 4) << "\n";

        msg.type = row_description;
        return msg;
    }

    message parse_data_row_message(const std::vector<uint8_t>& buffer, uint32_t& offset) {
        message msg{};
        std::cout << "MESSAGE: data row\n";


        const uint8_t* ptr = buffer.data() + 1 + offset;
        if (buffer.size() - offset < 6) {
            std::cout << "ERROR: Invalid message length: " << buffer.size() << "\n";
            return msg;
        }

        auto len = reverse<uint32_t>(ptr);

        if (len > buffer.size() - offset) {
            std::cout << "ERROR: Invalid provided message length: " << len << "\n";
            return msg;
        }

        offset += len + 1;
        msg.data.resize(len + 1);
        memcpy(msg.data.data(), ptr - 1, len + 1);

        std::cout << "    INFO: unparsed for now, part of raw data: " << string_from_bytes(ptr + 4, len - 4) << "\n";

        msg.type = data_row;
        return msg;
    }

    message parse_close_message(const std::vector<uint8_t>& buffer, uint32_t& offset) {
        message msg{};
        std::cout << "MESSAGE: close\n";


        const uint8_t* ptr = buffer.data() + 1 + offset;
        if (buffer.size() - offset < 6) {
            std::cout << "ERROR: Invalid message length: " << buffer.size() << "\n";
            return msg;
        }

        auto len = reverse<uint32_t>(ptr);

        if (len > buffer.size() - offset) {
            std::cout << "ERROR: Invalid provided message length: " << len << "\n";
            return msg;
        }

        offset += len + 1;
        msg.data.resize(len + 1);
        memcpy(msg.data.data(), ptr - 1, len + 1);

        ptr += 4;
        std::cout << "    INFO: type byte: " << (char)*ptr << "\n";
        std::cout << "    INFO: name of thing to close: " << (char*)ptr << "\n";

        msg.type = close;
        return msg;
    }

    std::vector<message> message::create_from_data(std::vector<uint8_t>&& buffer) {
        std::vector<message> messages{};

        if (buffer.empty())
            return messages;

        uint32_t offset = 0;
        while (offset < buffer.size()) {
            message msg{};

            switch (buffer[offset]) {
            case 'R':
                msg = parse_authentication_message(buffer, offset);
                break;
            case 'S':
                msg = parse_parameter_status_message(buffer, offset);
                break;
            case 'K':
                msg = parse_backend_key_data_message(buffer, offset);
                break;
            case 'Z':
                msg = parse_ready_for_query_message(buffer, offset);
                break;
            case 'T':
                msg = parse_row_description_message(buffer, offset);
                break;
            case 'D':
                msg = parse_data_row_message(buffer, offset);
                break;
            case 'C':
                msg = parse_close_message(buffer, offset);
                break;
            default:
                break;
            }

            if (msg.type != unknown)
                messages.emplace_back(std::move(msg));
            else {
                std::cout << "ERROR: Unknown message type! Aborting parsing of further messages in the packet!\n";
                return messages;
            }
        }
        return messages;
    }

    uint8_t get_frontend_message_first_byte(message_type type) {
        switch (type) {
        case sasl_initial_response:
        case sasl_response:
            return 'p';
        case query:
            return 'Q';
        case terminate:
            return 'X';
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



    std::string message::authentication_sasl_get_method_string() const {
        return string_from_bytes(data.data() + 9, data.size() - 11);
    }

    std::string message::authentication_sasl_continue_get_challenge() const {
        return string_from_bytes(data.data() + 9, data.size() - 9);
    }

    std::string message::authentication_sasl_final_get_additional_data() const {
        return string_from_bytes(data.data() + 9, data.size() - 9);
    }

    std::vector<field_def> message::row_description_get_field_defs() const {
        std::cout << "PARSING: Parsing row definition\n";

        auto* ptr = data.data() + 5;
        // this pointer points to the first byte after the buffer so that overflows can be caught
        const auto* overflow_ptr = data.data() + data.size();

        auto field_amount = reverse<uint16_t>(ptr);
        ptr += sizeof(field_amount);
        std::cout << "    INFO: Amount of fields: " << field_amount << "\n";

        std::vector<field_def> fields{};
        fields.reserve(field_amount);
        for (uint16_t n = 0; n < field_amount; n++) {
            std::cout << "  PARSING: processing field definition: " << n + 1 << "/" << field_amount << "\n";
            field_def field{};
            uint16_t max_len = 0;

            auto name_length = strnlen(reinterpret_cast<const char*>(ptr), max_len = overflow_ptr - ptr);
            if (name_length == max_len) {
                std::cout << "ERROR: Invalid string termination!\n";
                return {};
            }

            field.name = std::unique_ptr<char>((char*)std::malloc((name_length + 1) * sizeof(char)));
            memcpy(field.name.get(), ptr, name_length + 1);
            ptr += name_length + 1;
            std::cout << "    INFO: field name: " << field.name.get() << "\n";

            field.table_oid = reverse<uint32_t>(ptr);
            ptr += sizeof(field.table_oid);
            std::cout << "    INFO: field table OID: " << field.table_oid << "\n";

            field.column_attribute = reverse<uint16_t>(ptr);
            ptr += sizeof(field.column_attribute);
            std::cout << "    INFO: field column attribute: " << field.column_attribute << "\n";

            field.data_type_oid = reverse<uint32_t>(ptr);
            ptr += sizeof(field.data_type_oid);
            std::cout << "    INFO: field data type OID: " << field.data_type_oid << "\n";

            field.data_size = reverse<uint16_t>(ptr);
            ptr += sizeof(field.data_size);
            std::cout << "    INFO: field data size: " << field.data_size << "\n";

            field.type_modifier = reverse<uint32_t>(ptr);
            ptr += sizeof(field.type_modifier);
            std::cout << "    INFO: field type modifier: " << field.type_modifier << "\n";

            field.format_code = reverse<uint16_t>(ptr);
            ptr += sizeof(field.format_code);
            std::cout << "    INFO: field format code: " << field.format_code << "\n";

            fields.push_back(std::move(field));
        }

        if (ptr > overflow_ptr) {
            std::cout << "ERROR: Message parsing led to buffer overflow!\n";
            return {};
        }

        return std::move(fields);
    }

    uint16_t message::data_row_get_row_number() const {
        return reverse<uint16_t>(data.data() + 5);
    }
}
