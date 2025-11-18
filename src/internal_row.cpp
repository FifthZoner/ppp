//
// Created by fifthzoner on 16/11/2025.
//

#include "internal_row.hpp"

#include "table.hpp"

namespace ppp::internal {
    void internal_row::set_data_from_table(const table& table) {
        _data.reserve(table.fields.size());
        for (auto& n : table.fields)
            _data.emplace_back(n.field_type, n.data_size);
    }

    internal_row::internal_row(const table& table) {
        set_data_from_table(table);
    }

    internal_row::internal_row(const table& table, message&& message) {
        set_data_from_table(table);

        if (message.type != data_row)
            throw std::runtime_error("Passed message is not a data row!");

        if (message.data_row_get_row_number() != table.fields.size())
            throw std::runtime_error("Passed message has different column amount from the table!");

        auto fields = message.data_row_get_values();

        for (auto n = 0; n < fields.size(); n++)
            _data[n] = fields[n];
    }

    internal_row::internal_row(const internal_row& other) {

    }

    internal_row::internal_row(internal_row&& other) noexcept {
        _data = std::move(other._data);
    }

    typeless_value& internal_row::operator[](const table& table, const std::string& name) {
        for (auto n = 0; n < table.fields.size(); n++)
            if (not strcmp(table.fields[n].name.get(), name.c_str()))
                return _data[n];
        throw std::runtime_error("Provided name does not name a column in table!");
    }

    typeless_value& internal_row::operator[](const table& table, std::size_t index) {
        if (index < table.fields.size())
            return _data[index];
        throw std::runtime_error("Provided column index exceeds table's column count!");

    }

     std::ostream& operator<<(std::ostream& os, internal_row& row) {
        os << "(";
        for (auto n = 0; n < row._data.size(); n++) {
            os << row._data[n];
            if (n != row._data.size() - 1)
                os << ", ";
        }
        return os << ")";
    }
} // ppp