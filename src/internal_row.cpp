//
// Created by fifthzoner on 16/11/2025.
//

#include "row.hpp"

namespace ppp::internal {

    internal_row::internal_row(const table& table) {
    }

    internal_row::internal_row(const table& table, message&& message) {
    }

    internal_row::internal_row(const internal_row& other) {
        throw std::exception();
    }

    internal_row::internal_row(internal_row&& other) noexcept {

    }

    internal_row_value internal_row::operator[](const table& table, const std::string& name) {
        return (*this)[table, table[name]];
    }

    internal_row_value internal_row::operator[](const table& table, std::size_t index) {
        return {table, *this, index};
    }

    internal_row_value::internal_row_value(const table& table, internal_row& row, std::size_t column_index)
        : _table(table), _row(row), _column_index(column_index)
    {}

} // ppp