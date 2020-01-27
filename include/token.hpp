#ifndef LCC_TOKEN_HPP__
#define LCC_TOKEN_HPP__

namespace lcc {
enum class TokenType {
    s_specifier, // s
    c_specifier, // c
    w_specifier, // w
    e_specifier, // e

    equal_conditional,   // ?
    greater_conditional, // ?>

    block_delimiter, // / (ends or starts expression, depends on context)

    integer,        // 123
    string_literal, // "..."

    char_expr,         // #CHAR#
    word_expr,         // #WORD#
    text_expr,         // #TEXT#
    input_length_expr, // #INPUT_LENGTH#
    stack_length_expr, // #STACK_LENGTH#

    push_to_stdout,     // .
    push_all_to_stdout, // !
    pop,                // <
    clear,              // <e

    plus,          // +
    minus,         // -
    ord,           // o
    chr,           // c
    duplicate,     // >
    duplicate_all, // >e

    exit_parent,    // ^
    exit_specifier, // ^^
    exit_program,   // ^^^

    change_stack, // []
    move_val,     // @[]

    // Labels are not tokens
    jump_to // =>
};

struct LCToken {};
} // namespace lcc

#endif