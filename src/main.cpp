#include "program.hpp"
#include <iostream>

int main() {
    lcc::Program prg = lcc::Program::from_string(
        R"(?/^/cc#INPUT_LENGTH#!@[]!/e/#STACK_LENGTH#/w/ococ@[]?>/         ^^/@[]#TEXT#o>e
#WORD#c+<e
@[]/)");
    for (auto& token : prg.tokens) { std::cout << (int)token.get_type() << ' '; }
    return 0;
}
