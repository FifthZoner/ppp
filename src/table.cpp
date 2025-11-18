//
// Created by fifthzoner on 16/11/2025.
//

#include "table.hpp"

#include <iostream>

namespace ppp {
    row_ref table::operator[](std::size_t index) {
        if (index >= rows.size())
            throw std::runtime_error("Row index exceeds table capacity");
        return {*this, rows[index]};
    }

    table::table(std::vector<internal::field_def>&& fields) {
        fields = std::move(fields);
    }

    table::table(std::vector<internal::field_def>&& fields, std::vector<internal::internal_row>&& rows) {
        fields = std::move(fields);
        // TODO: add match validation or just convert to table move actually
        rows = std::move(rows);
    }

    table::table(std::vector<internal::field_def>&& given_fields, std::vector<internal::message>&& row_messages) {
        fields = std::move(given_fields);

        std::size_t counter = 0;
        for (auto row : row_messages) {
            if (row.type != internal::data_row)
                continue;
            ++counter;
            if (row.data_row_get_row_number() != fields.size())
                throw std::runtime_error("Invalid row column amount");
        }

        rows.reserve(counter);
        for (auto row : row_messages) {
            if (row.type != internal::data_row)
                continue;
            std::cout << "  PARSING: parsing data row\n";
            rows.emplace_back(*this, std::move(row));
            internal::internal_row& parsed = rows[rows.size() - 1];
            std::cout << "    INFO: Parsed row: " << parsed << "\n";
        }
    }

    std::ostream& operator<< (std::ostream& os, table& table) {
        for (auto& n : table.rows) {
            if (&n != table.rows.data())
                os << ",\n";
            os << n;
        }
        return os << "\n";
    }
} // ppp