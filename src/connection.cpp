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

            std::cout << "Received " << result.size() << " bytes:\n";
            for (unsigned char n : result)
                std::cout << std::hex << static_cast<unsigned>(n) << " ";
            std::cout << std::dec << "\n";
        }
        catch (const std::system_error& e) {
            std::cerr << "Error while receiving the response: " << e.what() << "\n";
        }
        return result;
    }

    connection::connection() {
        asio::io_service io_service;
        tcp::socket socket(io_service);
        socket.connect(tcp::endpoint(address::from_string("192.168.0.233"), 5432));

        if (socket.is_open())
            std::cout << "Success!\n";
        else
            std::cout << "Error!\n";

        auto msg = message::create_startup_message("post");

        std::cout << "Sent: " << socket.send(asio::buffer(msg.data)) << " bytes of message\n";

        auto res = message::create_from_data(receive_message(socket));

        if (res.type != authentication_sasl) {
            std::cout << "Expected sasl auth!";
            return;
        }

        auto algorithmList = res.authentication_sasl_get_method_string();

        if (algorithmList == "SCRAM-SHA-256") {
            std::cout << "Parsed response as supported: " << algorithmList << " SASL auth mechanism!\n";
        }
        else std::cout << "Unimplemented sasl algorithm: " << algorithmList << "\n";

        Gsasl *ctx = NULL;
        auto rc = gsasl_init(&ctx);

        if (rc != GSASL_OK) {
            std::cout << "GNU sasl init failed!\n";
            return;
        }

        Gsasl_session* session;

        if ((rc = gsasl_client_start (ctx, algorithmList.c_str(), &session)) != GSASL_OK) {
            std::cout << "Cannot initialize client: %s\n" << gsasl_strerror (rc) << "\n";
            return;
        }
        std::cout << "Initialized gnu sasl!\n";

        gsasl_property_set (session, GSASL_AUTHID, "post");
        gsasl_property_set (session, GSASL_PASSWORD, "post");

        char buf[BUFSIZ] = "";
        char* p = nullptr;
        std::size_t out_len;

        memcpy(buf, "post", 4);
        rc = gsasl_step (session, buf, 4, &p, &out_len);
        if (rc != GSASL_OK and rc != GSASL_NEEDS_MORE) {
            std::cout << "Encoding error: " << gsasl_strerror (rc) << "\n";
            return;
        }

        std::string first_message = {p};
        free(p);

        std::vector<uint8_t> encoded{};
        encoded.resize(first_message.size());
        memcpy(encoded.data(), first_message.data(), encoded.size());

        std::cout << "Output: " << first_message << '\n';


        msg = message::create_sasl_initial_response("SCRAM-SHA-256", std::move(encoded));
        std::cout << "Sent: " << socket.send(asio::buffer(msg.data)) << " bytes of message\n";

        res = message::create_from_data(receive_message(socket));

        if (res.type != authentication_sasl_continue) {
            std::cout << "Expected sasl auth continuation!";
            gsasl_finish (session);
            gsasl_done(ctx);
            return;
        }

        auto challenge = res.authentication_sasl_continue_get_challenge();
        std::cout << "Parsed response as challenge with value of: " << challenge << "\n";

        memcpy(buf, challenge.c_str(), challenge.size());
        rc = gsasl_step (session, buf, challenge.size(), &p, &out_len);
        if (rc != GSASL_OK and rc != GSASL_NEEDS_MORE) {
            std::cout << "Encoding error: " << gsasl_strerror (rc) << "\n";
            return;
        }


        encoded = {};
        encoded.resize(strlen(p));
        memcpy(encoded.data(), p, encoded.size());

        std::cout << "Output: " << p << '\n';
        free(p);

        msg = message::create_sasl_response(std::move(encoded));
        std::cout << "Sent: " << socket.send(asio::buffer(msg.data)) << " bytes of message\n";

        res = message::create_from_data(receive_message(socket));

        if (res.type != authentication_sasl_final) {
            std::cout << "Expected sasl auth final message!";
            gsasl_finish (session);
            gsasl_done(ctx);
            return;
        }

        auto additional_data = res.authentication_sasl_final_get_additional_data();
        std::cout << "Parsed response as challenge with value of: " << additional_data << "\n";


        gsasl_finish (session);
        gsasl_done(ctx);
    }
}
