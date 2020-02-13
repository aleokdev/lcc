#include "lcc/program.hpp"
#include <iostream>

int main() {
    lcc::Program prg = lcc::Program::from_string(
        R"(e/-/c/ #TEXT##STACK_LENGTH##STACK_LENGTH#.?>/+/!#TEXT#o/e/!-    #CHAR#
#TEXT#/e/c?>/--o+o.#TEXT#!@[]^^//)");
    for (auto& token : prg.tokens) { std::cout << (int)token.get_type() << ' '; }
    return 0;
}
