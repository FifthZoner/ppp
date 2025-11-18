//
// Created by fifthzoner on 16/11/2025.
//

#ifndef PPP_ROW_HPP
#define PPP_ROW_HPP
#include "internal_row.hpp"

namespace ppp {
    class row_ref;
    class table;

    class row {
        const table& _table;
        internal::internal_row _row;
    public:
        explicit row(const table& table);
        row(const table& table, const row& row);
        row(const row& other);
        explicit row(const row_ref& other);
        row(row&& other) noexcept ;
        explicit row(row_ref&& other);

        internal::typeless_value& operator[](const std::string& name);
        internal::typeless_value& operator[](std::string&& name);
        internal::typeless_value& operator[](std::size_t index);
    };

    class row_ref {
        const table& _table;
        internal::internal_row& _row;
    public:
        row_ref(const table& table, internal::internal_row& row);
        row_ref(const row_ref& row_ref);
        row_ref(row_ref&& row_ref) noexcept;

        internal::typeless_value& operator[](const std::string& name) const;
        internal::typeless_value& operator[](std::string&& name) const;
        internal::typeless_value& operator[](std::size_t index) const;

        friend class row;
    };
}


#endif //PPP_ROW_HPP