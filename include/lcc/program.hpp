#ifndef LCC_PROGRAM_HPP__
#define LCC_PROGRAM_HPP__

#include <filesystem>
#include <lcc/lexer_state.hpp>
#include <lcc/token.hpp>
#include <string_view>
#include <vector>

#include <sstream>

namespace lcc {
class Program {
public:
    Program() = default;

    static Program from_string(const std::string&);
    static Program from_file(std::filesystem::path);

    using TokenIndex = size_t;
    std::vector<Token> tokens;
    std::unordered_map<std::string, TokenIndex> labels;
    LexerState lexer_state;
    size_t lexer_pc;

private:
    Token parse_next(std::istringstream&);
};
} // namespace lcc

#endif