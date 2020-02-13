#ifndef LCC_LEXER_STATE_HPP
#define LCC_LEXER_STATE_HPP

namespace lcc {
    enum class LexerState {
        outside_specificator,
        inside_block // For when inside a specificator / conditional / etc
    };
}

#endif //LCC_LEXER_STATE_HPP
