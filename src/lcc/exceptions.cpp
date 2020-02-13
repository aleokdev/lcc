#include "lcc/exceptions.hpp"
#include <lcc/program.hpp>
#include <cstdio>

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
} // namespace lcc