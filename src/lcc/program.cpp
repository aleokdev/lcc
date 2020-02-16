#include <lcc/exceptions.hpp>
#include <lcc/program.hpp>

#include <algorithm>
#include <sstream>
#include <string>

namespace lcc {

Token Program::parse_next(std::istringstream& s) {
    std::string cur_token;
    while (::isspace(s.peek())) { s.get(); }

    if (s.peek() == string_delimiter) {
        s.get(); // Skip string delimiter
        // Parse string
        while (s.peek() != string_delimiter) {
            cur_token += (char)s.get();
            if (s.eof())
                throw lcc::UnexpectedEOFError(*this);
        }
        s.get(); // Skip string delimiter
        return Token(TokenType::string_literal, cur_token);
    } else if (::isdigit(s.peek())){
        int num = 0;
        // Parse integer
        while(isalnum(s.peek()))
        {
            num *= 10;
            num += s.get() - '0';
        }
        return Token(TokenType::integer, num);
    }
    else
        for (;;) {
            cur_token += (char)s.get();
            lexer_pc++;
            if (token_bindings.find(cur_token) != token_bindings.end() &&
                token_bindings.find(cur_token + (char)s.peek()) == token_bindings.end()) {
                return Token(token_bindings[cur_token]);
            } else if (s.eof() || cur_token.size() > max_token_size)
                throw UnidentifiedTokenError(*this);

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
        if ((size_t)prg.tokens.back().get_type() & (size_t)TokenType::block_starter)
            scope_level++;
        else if ((size_t)prg.tokens.back().get_type() & (size_t)TokenType::block_ender) {
            if (scope_level == 0)
                throw SyntaxError(
                    prg, "Used block ender outside of scope (Check for extra slashes '/')");
            scope_level--;
        } else {                  // Any command
            if (scope_level == 0) // Outside specifier
                throw SyntaxError(prg, "Used command outside specifier");
        }
    }
    if (scope_level != 0)
        throw SyntaxError(prg, "Scope does not end at EOF (Did you forget a '/' character?)");

    return prg;
}

} // namespace lcc