#ifndef LCC_TOKEN_HPP__
#define LCC_TOKEN_HPP__

#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>

namespace lcc {
enum class TokenType {
    specifier = 1 << 5,                            // Flag
    block_starter = 1 << 6,                        // Flag
    block_ender = 1 << 7,                          // Flag
    s_specifier = (specifier | block_starter) + 1, // s/
    c_specifier = (specifier | block_starter) + 2, // c/
    w_specifier = (specifier | block_starter) + 3, // w/
    e_specifier = (specifier | block_starter) + 4, // e/

    equal_conditional = block_starter + 1,   // ?/
    greater_conditional = block_starter + 2, // ?>/

    end_code_block = block_ender + 1, // / (ends expression)
    exit_parent = block_ender + 2,    // ^/
    exit_specifier = block_ender + 3, // ^^/
    exit_program = block_ender + 4,   // ^^^/

    integer = 1,    // 123
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

    plus,      // +
    minus,     // -
    multiply,  // *
    power,     // ^
    ord,       // o
    chr,       // c
    duplicate, // >

    change_stack, // []
    move_val,     // @[]

    // Labels are not tokens
    jump_to // =>
};

inline size_t operator&(TokenType t1, TokenType t2) { return ((size_t)t1 & (size_t)t2); }
inline size_t operator&(size_t t1, TokenType t2) { return (t1 & (size_t)t2); }

static constexpr char label_delimiter = ':';
static constexpr char string_delimiter = '"';
static constexpr int max_token_size = 16;
static inline auto token_bindings = std::unordered_map<std::string_view, TokenType>(
    {{"s/", TokenType::s_specifier},
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
     {"*", TokenType::multiply},
     {"^", TokenType::power},
     {"o", TokenType::ord},
     {"c", TokenType::chr},
     {">", TokenType::duplicate},
     {"^/", TokenType::exit_parent},
     {"^^/", TokenType::exit_specifier},
     {"^^^/", TokenType::exit_program},
     {"[]", TokenType::change_stack},
     {"@[]", TokenType::move_val},
     {"=>", TokenType::jump_to}});

class Token {
private:
    TokenType type;
    std::variant<std::string, int> value;

public:
    Token() = default;
    explicit Token(TokenType t) : type(t) {}
    template<typename V> Token(TokenType t, V val) : type(t), value(val) {}

    [[nodiscard]] const TokenType& get_type() const { return type; }

    template<typename T>[[nodiscard]] T get_value_as() { return std::get<T>(value); }
};
} // namespace lcc

#endif