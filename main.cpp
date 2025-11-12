//
// Created by fifthzoner on 02/11/2025.
//

#include "ppp.hpp"
#include <gsasl.h>
#include <iostream>

int main() {
    ppp::connection conn{asio::ip::address::from_string("10.147.17.126"), 5432, "post", "post", "microviewer"};

    conn.query("select * from boards;");

    return 0;
}
