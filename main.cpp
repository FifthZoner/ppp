//
// Created by fifthzoner on 02/11/2025.
//

#include "ppp.hpp"

int main() {
    ppp::connection conn{asio::ip::address::from_string("10.147.17.126"), 5432, "post", "post", "microviewer"};

    auto table = conn.query("select * from manufacturers;");
    std::cout << table.rows[1][table, "man_id"] << "\n";
    table.rows[1][table, "man_id"] = "2137.2137";
    std::cout << table.rows[1][table, "man_id"] << "\n";

    return 0;
}
