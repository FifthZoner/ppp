//
// Created by fifthzoner on 02/11/2025.
//

#include "ppp.hpp"

int main() {
    ppp::connection conn{asio::ip::address::from_string("10.147.17.126"), 5432, "post", "post", "microviewer"};

    auto table = conn.query("select * from manufacturers;");
    std:: cout << table.rows[1][table, "man_id"].operator()<int>() << "\n";
    table.rows[1][table, "man_id"].operator=<int>(10);
    std:: cout << table.rows[1][table, "man_id"].operator()<int>() << "\n";

    return 0;
}
