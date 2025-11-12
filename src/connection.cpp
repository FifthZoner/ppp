//
// Created by fifthzoner on 02/11/2025.
//

#include "connection.hpp"
#include "message.hpp"
#include <asio.hpp>
#include <iostream>

#include <gsasl.h>

using namespace std::chrono_literals;
using namespace ppp::internal;
using namespace asio::ip;

namespace ppp {
    std::vector<uint8_t> receive_message(tcp::socket& socket) {
        std::vector<uint8_t> result{};

        try {
            std::array<uint8_t, 256> buffer{};
            std::size_t len = 0;
            do {
                len = socket.read_some(asio::buffer(buffer));
                auto old_size = result.size();
                result.resize(result.size() + len);
                for (std::size_t n = 0; n < len; ++n)
                    result[old_size + n] = buffer[n];
            }
            while (len == buffer.size());

            std::cout << "PACKET: Received " << result.size() << " bytes: ";
            for (unsigned char n : result)
                std::cout << std::hex << static_cast<unsigned>(n) << " ";
            std::cout << std::dec << "\n";
        }
        catch (const std::system_error& e) {
            std::cout << "ERROR: Error while receiving the response: " << e.what() << "\n";
        }
        return result;
    }

    connection::connection(address&& ip_address, uint16_t port, std::string&& user, std::string&& password, std::string&& database) {

        socket = std::make_unique<tcp::socket>(io_service);

        socket->connect(tcp::endpoint(ip_address, port));

        if (socket->is_open())
            std::cout << "CONNECTION: Opened connection!\n";
        else
            std::cout << "ERROR: Error during socket opening!\n";

        auto msg = message::create_startup_message(std::move(user), std::move(database));

        std::cout << "CONNECTION: Sent: " << socket->send(asio::buffer(msg.data)) << " bytes of message\n";

        auto res = message::create_from_data(receive_message(*socket))[0];

        if (res.type != authentication_sasl) {
            std::cout << "ERROR: Expected sasl auth!";
            return;
        }

        auto algorithmList = res.authentication_sasl_get_method_string();

        if (algorithmList == "SCRAM-SHA-256") {
            std::cout << "AUTH: Parsed response as supported: " << algorithmList << " SASL auth mechanism!\n";
        }
        else std::cout << "ERROR: Unimplemented sasl algorithm: " << algorithmList << "\n";

        Gsasl *ctx = nullptr;
        auto rc = gsasl_init(&ctx);

        if (rc != GSASL_OK) {
            std::cout << "ERROR: GNU sasl init failed!\n";
            return;
        }

        Gsasl_session* session;

        if ((rc = gsasl_client_start (ctx, algorithmList.c_str(), &session)) != GSASL_OK) {
            std::cout << "ERROR: Cannot initialize client: %s\n" << gsasl_strerror (rc) << "\n";
            return;
        }
        std::cout << "AUTH: Initialized gnu sasl!\n";

        gsasl_property_set (session, GSASL_AUTHID, user.c_str());
        gsasl_property_set (session, GSASL_PASSWORD, password.c_str());

        char buf[BUFSIZ] = "";
        char* p = nullptr;
        std::size_t out_len;

        memcpy(buf, password.c_str(), password.length());
        rc = gsasl_step (session, buf, password.length(), &p, &out_len);
        if (rc != GSASL_OK and rc != GSASL_NEEDS_MORE) {
            std::cout << "ERROR: Encoding error: " << gsasl_strerror (rc) << "\n";
            return;
        }

        std::string first_message = {p};
        free(p);

        std::vector<uint8_t> encoded{};
        encoded.resize(first_message.size());
        memcpy(encoded.data(), first_message.data(), encoded.size());

        msg = message::create_sasl_initial_response("SCRAM-SHA-256", std::move(encoded));
        std::cout << "CONNECTION: Sent: " << socket->send(asio::buffer(msg.data)) << " bytes of message\n";

        res = message::create_from_data(receive_message(*socket))[0];

        if (res.type != authentication_sasl_continue) {
            std::cout << "ERROR: Expected sasl auth continuation!";
            gsasl_finish (session);
            gsasl_done(ctx);
            return;
        }

        auto challenge = res.authentication_sasl_continue_get_challenge();

        memcpy(buf, challenge.c_str(), challenge.size());
        rc = gsasl_step (session, buf, challenge.size(), &p, &out_len);
        if (rc != GSASL_OK and rc != GSASL_NEEDS_MORE) {
            std::cout << "ERROR: Encoding error: " << gsasl_strerror (rc) << "\n";
            return;
        }


        encoded = {};
        encoded.resize(strlen(p));
        memcpy(encoded.data(), p, encoded.size());
        free(p);

        msg = message::create_sasl_response(std::move(encoded));
        std::cout << "CONNECTION: Sent: " << socket->send(asio::buffer(msg.data)) << " bytes of message\n";

        auto responses = message::create_from_data(receive_message(*socket));

        if (responses.empty() or responses[0].type != authentication_sasl_final) {
            std::cout << "ERROR: Expected sasl auth final message!";
            gsasl_finish (session);
            gsasl_done(ctx);
            return;
        }

        std::cout << "CONNECTION: Successfully connected to database!\n";

        gsasl_finish (session);
        gsasl_done(ctx);

        is_authed = true;
    }

    void connection::query(std::string&& query) {
        auto msg = message::create_query_message(std::move(query));
        std::cout << "CONNECTION: Sent: " << socket->send(asio::buffer(msg.data)) << " bytes of message\n";

        auto responses = message::create_from_data(receive_message(*socket));
        for (auto& n : responses)
            if (n.type == row_description)
                auto x = n.row_description_get_field_defs();
    }

    connection::connection(connection&& other) noexcept {
        socket = std::move(other.socket);
        is_authed = other.is_authed;
        // TODO: what about the io service? unique_ptr?
    }

    connection::~connection() {
        if (socket != nullptr and socket->is_open()) {
            if (is_authed) {
                auto msg = message::create_terminate_message();
                std::cout << "CONNECTION: Sent: " << socket->send(asio::buffer(msg.data)) << " bytes of termination message\n";
            }
            socket->close();
        }
        std::cout << "CONNECTION: Terminated connection!\n";
    }
}
