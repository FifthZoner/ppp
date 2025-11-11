//
// Created by fifthzoner on 02/11/2025.
//

#ifndef PPP_CONNECTION_HPP
#define PPP_CONNECTION_HPP

#include <asio.hpp>

namespace ppp {
    class connection {
        asio::io_service io_service{};
        std::unique_ptr<asio::ip::tcp::socket> socket = nullptr;
        bool is_authed = false;


    public:
        void query(std::string&& query);

        connection(asio::ip::address&& ip_address, uint16_t port, std::string&& user, std::string&& password, std::string&& database);
        connection(connection&& other) noexcept ;
        ~connection();
    };
}

#endif //PPP_CONNECTION_HPP