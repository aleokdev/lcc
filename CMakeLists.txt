# CMakcmake_minimum_required (VERSION 3.8)

project (lcc LANGUAGE C++)

add_executable (lcc "src/main.cpp")
set_target_properties(lcc PROPERTIES
            CXX_STANDARD 17
            )