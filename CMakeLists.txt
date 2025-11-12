cmake_minimum_required(VERSION 4.0)
project(ppp)

set(CMAKE_CXX_STANDARD 23)

add_library(ppp STATIC
        ppp.hpp
        src/connection.hpp
        src/connection.hpp
        src/connection.cpp
        src/message.cpp
        src/message.hpp
        src/utils.tpp
        src/field_def.cpp
        src/field_def.hpp)

set_target_properties(ppp PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(ppp gsasl)

add_executable(ppp_test main.cpp)
add_dependencies(ppp_test ppp)
target_link_libraries(ppp_test ppp)
