#include "lcc/exceptions.hpp"
#include "lcc/instance.hpp"
#include <algorithm>
#include <cstdio>
#include <lcc/program.hpp>

namespace lcc {
[[nodiscard]] const char* SyntaxError::what() const noexcept {
    char* buf = (char*)malloc(64);
    std::sprintf(buf, "Syntax error at position %zu: %s", prg->lexer_pc, message.c_str());
    return buf;
}

[[nodiscard]] const char* UnexpectedEOFError::what() const noexcept {
    char* buf = (char*)malloc(64);
    std::sprintf(buf, "Expected token at EOF");
    return buf;
}

[[nodiscard]] const char* UnidentifiedTokenError::what() const noexcept {
    char* buf = (char*)malloc(64);
    std::sprintf(buf, "Unidentified token at position %zu", prg->lexer_pc);
    return buf;
}
const char* NotEnoughStackItemsError::what() const noexcept {
    char* buf = (char*)malloc(64);
    std::sprintf(
        buf, "Not enough stack items to perform '%s' command",
        std::find_if(token_bindings.begin(), token_bindings.end(), [&](auto& t) -> bool {
            return instance->get_pc_token().get_type() == t.second;
        })->first.data());
    return buf;
}
const char* ValueTypeError::what() const noexcept {
    char* buf = (char*)malloc(64);
    std::sprintf(
        buf, "Type error: '%s' command requires %s args, but %s was used instead",
        std::find_if(token_bindings.begin(), token_bindings.end(),
                     [&](auto& t) -> bool { return instance->get_pc_token().get_type() == t.second; })
            ->first.data(),
        value_type_names[expected_type].data(),
        value_type_names[instance->get_current_stack().top().get_value_type()].data());
    return buf;
}
} // namespace lcc