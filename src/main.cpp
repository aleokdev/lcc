#include "lcc/program.hpp"
#include <iostream>
#include <lcc/instance.hpp>

int main() {
    lcc::Program prg = lcc::Program::from_string(
        R"(s/"Starting!"./ c/#CHAR#./ e/"Ending!"./)");
    for (auto& token : prg.tokens) { std::cout << (int)token.get_type() << ' '; }
    lcc::Instance(prg).run("Testing, testing...");
    return 0;
}
