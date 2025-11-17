//
// Created by fifthzoner on 02/11/2025.
//

#ifndef PPP_CONNECTION_HPP
#define PPP_CONNECTION_HPP

#include <asio.hpp>

#include "postgresql_types.hpp"

namespace ppp {
    struct type_definition {
        std::string _name{};
        uint32_t _oid = 0;
        internal::type_enum _type = internal::none;
    };

    class connection {
        asio::io_service io_service{};
        std::unique_ptr<asio::ip::tcp::socket> socket = nullptr;
        bool is_authed = false;

        std::unordered_map<std::string, type_definition> db_types {};


    public:
        void query(std::string&& query);

        connection(asio::ip::address&& ip_address, uint16_t port, std::string&& user, std::string&& password, std::string&& database);
        connection(connection&& other) noexcept ;
        ~connection();
    };
}

#endif //PPP_CONNECTION_HPP