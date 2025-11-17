//
// Created by fifthzoner on 12/11/2025.
//

#include "field_def.hpp"

#include <cstring>

namespace ppp::internal {
    field_def::field_def(const field_def& other) {
        if (other.name != nullptr) {
            auto length = strlen(other.name.get());
            name = std::unique_ptr<char>(new char[length + 1]);
            memcpy(name.get(), other.name.get(), length + 1);
        }

        table_oid = other.table_oid;
        data_type_oid = other.data_type_oid;
        type_modifier = other.type_modifier;
        column_attribute = other.column_attribute;
        data_size = other.data_size;
        format_code = other.format_code;
    }
}
