//
// Created by fifthzoner on 12/11/2025.
//

#ifndef PPP_FIELD_DEF_HPP
#define PPP_FIELD_DEF_HPP
#include <memory>

#include "postgresql_types.hpp"


namespace ppp::internal {

    struct field_def {
        std::unique_ptr<char> name = nullptr;
        uint32_t table_oid = 0;
        uint32_t data_type_oid = 0;
        uint32_t type_modifier = 0;
        uint16_t column_attribute = 0;
        int16_t data_size = 0;
        uint16_t format_code = 0;
        type_enum field_type = none;

        field_def() = default;
        field_def(const field_def& other);
        field_def(field_def&& other) noexcept = default;
    };

}

#endif //PPP_FIELD_DEF_HPP