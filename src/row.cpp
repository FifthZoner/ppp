//
// Created by fifthzoner on 16/11/2025.
//

#include "row.hpp"

ppp::row_ref::row_ref(const table& table, internal::internal_row& row) : _table(table), _row(row) {

}

ppp::row_ref::row_ref(const row_ref& row_ref) = default;

ppp::row_ref::row_ref(row_ref&& row_ref) noexcept = default;

ppp::internal::typeless_value& ppp::row_ref::operator[](const std::string& name) const {
    return _row[_table, name];
}

ppp::internal::typeless_value& ppp::row_ref::operator[](std::string&& name) const {
    return _row[_table, name];
}

ppp::internal::typeless_value& ppp::row_ref::operator[](std::size_t index) const {
    return _row[_table, index];
}
