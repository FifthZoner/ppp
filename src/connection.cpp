//
// Created by fifthzoner on 02/11/2025.
//

#include "connection.hpp"
#include "message.hpp"
#include <asio.hpp>
#include <iostream>

#include <gsasl.h>

#include "table.hpp"

using namespace std::chrono_literals;
using namespace ppp::internal;
using namespace asio::ip;

namespace ppp {
    connection::connection(address&& ip_address, uint16_t port, std::string&& user, std::string&& password, std::string&& database) {

        socket = std::make_unique<tcp::socket>(io_service);

        socket->connect(tcp::endpoint(ip_address, port));

        if (socket->is_open())
            std::cout << "CONNECTION: Opened connection!\n";
        else
            std::cout << "ERROR: Error during socket opening!\n";

        auto msg = message::create_startup_message(std::move(user), std::move(database));

        std::cout << "CONNECTION: Sent: " << socket->send(asio::buffer(msg.data)) << " bytes of message\n";

        auto res = message::create_from_data(*socket)[0];

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

        res = message::create_from_data(*socket)[0];

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

        auto responses = message::create_from_data(*socket);

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

        std::cout << "CONNECTION: Querying type OIDs...";

        msg = message::create_query_message("SELECT oid, typname FROM pg_type where typrelid = 0 and typcategory != 'A';");
        std::cout << "CONNECTION: Sent: " << socket->send(asio::buffer(msg.data)) << " bytes of message\n";

        responses = message::create_from_data(*socket);

        for (auto& n : responses) {
            if (n.type == row_description) {
                auto fields = n.row_description_get_field_defs();
                for (auto& f : fields) {
                    if (std::string(f.name.get()) == "oid") {
                        type_definition current{};

                        current._name = "oid";
                        current._oid = f.data_type_oid;
                        f.field_type = current._type = enum_postgresql_type_integer_4;


                        this->db_types.emplace(current._oid, current);
                    }
                    else if (std::string(f.name.get()) == "typname") {
                        type_definition current{};

                        current._name = "name";
                        current._oid = f.data_type_oid;
                        f.field_type = current._type = enum_postgresql_type_varchar_n;

                        this->db_types.emplace(current._oid, current);
                    }
                }
                auto type_table = table(std::move(fields), std::move(responses));

                for (auto& x : type_table.rows) {
                    auto name = x[type_table, "typname"].operator()<std::string>();
                    auto oid = x[type_table, "oid"].operator()<uint32_t>();
                    if (not this->db_types.contains(oid)) {
                        type_definition current{};

                        current._type = get_type_enum_by_name(name);
                        current._name = std::move(name);
                        x[type_table, "oid"](current._oid);


                        this->db_types.emplace(oid, current);
                    }
                }

                break;
            }
        }

    }

    table connection::query(std::string&& query) {
        auto msg = message::create_query_message(std::move(query));
        std::cout << "CONNECTION: Sent: " << socket->send(asio::buffer(msg.data)) << " bytes of message\n";

        auto responses = message::create_from_data(*socket);
        for (auto& n : responses)
            if (n.type == row_description) {
                auto x = n.row_description_get_field_defs();
                for (auto& y : x)
                    y.field_type = db_types[y.data_type_oid]._type;
                return table(std::move(x), std::move(responses));
            }
        throw std::runtime_error("Could not parse a table!\n");
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
