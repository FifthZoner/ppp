//
// Created by fifthzoner on 02/11/2025.
//

#include "ppp.hpp"

int main() {
    ppp::connection conn{asio::ip::address::from_string("10.147.17.126"), 5432, "post", "post", "microviewer"};

    auto table = conn.query("select * from manufacturers;");
    //std::cout << table[1]["man_id"] << "\n";
    //table[1]["man_id"] = "2137.2137";
    //table[1]["man_id"] = "12415152";
    //table[1]["man_id"] = 124u;
    //table[1]["man_id"] = 21.37f;
    //table[1]["man_id"] = -50;
    //table[1]["man_id"] = "-1";
    //std::cout << table[1]["man_id"] << "\n";

    std::cout << table;

    return 0;
}
