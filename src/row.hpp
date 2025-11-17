//
// Created by fifthzoner on 16/11/2025.
//

#ifndef PPP_ROW_HPP
#define PPP_ROW_HPP
#include "table.hpp"

namespace ppp {
    class row_ref;

    class row {
        const table& _table{};
        internal::internal_row _row;
    public:
        explicit row(const table& table);
        row(const table& table, const row& row);
        row(const row& other);
        explicit row(const row_ref& other);
        row(row&& other) noexcept ;
        explicit row(row_ref&& other);
    };

    class row_ref {
        const table& _table{};
        internal::internal_row& _row;
    public:
        row_ref(const table& table, internal::internal_row& row);
        row_ref(const row_ref& row_ref);
        row_ref(row_ref&& row_ref) noexcept;

        friend class row;
    };
}


#endif //PPP_ROW_HPP