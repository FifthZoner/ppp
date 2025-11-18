//
// Created by fifthzoner on 16/11/2025.
//

#ifndef PPP_TABLE_HPP
#define PPP_TABLE_HPP
#include <vector>

#include "field_def.hpp"
#include "message.hpp"
#include "internal_row.hpp"
#include "row.hpp"

namespace ppp {
    /// Represents a table gotten from database as query response
    class table {
        // TODO: remove later
    public:
        std::vector<internal::field_def> fields{};
        std::vector<internal::internal_row> rows{};

        friend struct internal::internal_row;


        table() = default;
        explicit table(std::vector<internal::field_def>&& fields);
        table(std::vector<internal::field_def>&& fields, std::vector<internal::internal_row>&& rows);
        table(std::vector<internal::field_def>&& fields, std::vector<internal::message>&& row_messages);

        row_ref operator[](std::size_t index);

        friend std::ostream& operator<< (std::ostream& os, table& table);
    };

    std::ostream& operator<< (std::ostream& os, table& table);
} // ppp

#endif //PPP_TABLE_HPP
