#include "program.hpp"
#include "exceptions.hpp"

#include <algorithm>
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

Program Program::from_string(std::string_view s) {
    Program prg;

    size_t pc = 0;
    std::string cur_token;
    while (pc < s.size()) {
        if (is_space(s[pc])) {
            pc++;
            continue;
        }

        cur_token += s[pc];
        if (token_bindings.find(cur_token) != token_bindings.end())
        {
            prg.tokens.emplace_back(token_bindings[cur_token]);
            cur_token.clear();
        }else if (cur_token.size() > max_token_size)
            throw SyntaxError(pc, cur_token);

        pc++;
    }

    return prg;
}

} // namespace lcc