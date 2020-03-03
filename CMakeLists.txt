cmake_minimum_required (VERSION 3.8)

set(LCC_VERSION "0.0.0")
set(LINECRYPT_VERSION "1.1")

project (lcc LANGUAGES CXX VERSION ${LCC_VERSION})

add_executable (lcc
        "src/main.cpp"
        "src/lcc/program.cpp"
        "src/lcc/exceptions.cpp"
        "include/lcc/lexer_state.hpp"
        "src/lcc/instance.cpp"
        "include/lcc/value.hpp"
        "include/lcc/multi_stack.hpp"
        )
set_target_properties(lcc PROPERTIES CXX_STANDARD 17)
target_include_directories(lcc PUBLIC "include")
target_compile_definitions(lcc PRIVATE LCC_VERSION="${LCC_VERSION}" LINECRYPT_VERSION="${LINECRYPT_VERSION}")