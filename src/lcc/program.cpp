#include <lcc/exceptions.hpp>
#include <lcc/program.hpp>

#include <algorithm>
#include <sstream>
#include <string>

bool is_space(char c) {
    switch (c) {
        case ' ':
        case '\t':
        case '\n': return true;
        default: return false;
    }
}

namespace lcc {

Token Program::parse_next(std::istringstream& s) {
    std::string cur_token;
    for(;;) {
        if (is_space(s.peek())) {
            s.get();
            continue;
        }
        cur_token += (char)s.get();
        lexer_pc++;
        if (token_bindings.find(cur_token) != token_bindings.end() && token_bindings.find(cur_token+(char)s.peek()) == token_bindings.end()) {
            return Token(token_bindings[cur_token]);
        } else if (cur_token.size() > max_token_size)
            throw UnidentifiedTokenError(*this);

        if (s.eof())
            throw lcc::UnexpectedEOFError(*this);

        if (s.bad())
            throw;
    }
}

Program Program::from_string(const std::string& s) {
    Program prg;

    // Lex & parse tokens
    std::istringstream stream(s);
    prg.lexer_pc = 0;
    size_t scope_level = 0;
    while (stream.peek() ^ std::char_traits<char>::eof()) {
        prg.tokens.emplace_back(prg.parse_next(stream));
        if((size_t)prg.tokens.back().get_type() & (size_t)TokenType::block_starter)
            scope_level++;
        else if ((size_t)prg.tokens.back().get_type() & (size_t)TokenType::block_ender)
        {
            if(scope_level == 0)
                throw SyntaxError(prg, "Used block ender outside of scope");
            scope_level--;
        } else { // Any command
            if(scope_level == 0) // Outside specifier
                throw SyntaxError(prg, "Used command outside specifier");
        }
    }
    if(scope_level != 0)
        throw SyntaxError(prg, "Scope does not end at EOF");

    return prg;
}

} // namespace lcc