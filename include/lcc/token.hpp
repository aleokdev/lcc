#ifndef LCC_TOKEN_HPP__
#define LCC_TOKEN_HPP__

#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>

namespace lcc {
enum class TokenType {
    s_specifier, // s/
    c_specifier, // c/
    w_specifier, // w/
    e_specifier, // e/

    equal_conditional,   // ?/
    greater_conditional, // ?>/

    end_code_block, // / (ends expression)

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

    exit_parent,    // ^/
    exit_specifier, // ^^/
    exit_program,   // ^^^/

    change_stack, // []
    move_val,     // @[]

    // Labels are not tokens
    jump_to // =>
};

static constexpr inline char label_delimiter = ':';
static constexpr inline int max_token_size = 16;
/* clang-format off */
static inline auto token_bindings = std::unordered_map<std::string_view, TokenType> ({
    {"s/", TokenType::s_specifier},
    {"c/", TokenType::c_specifier},
    {"w/", TokenType::w_specifier},
    {"e/", TokenType::e_specifier},
    {"?/", TokenType::equal_conditional},
    {"?>/", TokenType::greater_conditional},
    {"/", TokenType::end_code_block},
    {"#CHAR#", TokenType::char_expr},
    {"#WORD#", TokenType::word_expr},
    {"#TEXT#", TokenType::text_expr},
    {"#INPUT_LENGTH#", TokenType::input_length_expr},
    {"#STACK_LENGTH#", TokenType::stack_length_expr},
    {".", TokenType::push_to_stdout},
    {"!", TokenType::push_all_to_stdout},
    {"<", TokenType::pop},
    {"<e", TokenType::clear},
    {"+", TokenType::plus},
    {"-", TokenType::minus},
    {"o", TokenType::ord},
    {"c", TokenType::chr},
    {">", TokenType::duplicate},
    {"^/", TokenType::exit_parent},
    {"^^/", TokenType::exit_specifier},
    {"^^^/", TokenType::exit_program},
    {"[]", TokenType::change_stack},
    {"@[]", TokenType::move_val},
    {"=>", TokenType::jump_to}
});
    /* clang-format on */

class Token {
private:
    TokenType type;
    std::variant<std::string, int> value;

public:
    Token() = default;
    explicit Token(TokenType t) : type(t) {}
    template<typename V> Token(TokenType t, V val) : type(t), value(val) {}

    [[nodiscard]] const TokenType& get_type() const { return type; }

    template<typename T> T get_value_as() { return std::get<T>(value); }
};
} // namespace lcc

#endif