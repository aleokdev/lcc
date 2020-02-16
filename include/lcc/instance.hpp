#ifndef LCC_INSTANCE_HPP
#define LCC_INSTANCE_HPP

#include "lcc/value.hpp"
#include "multi_stack.hpp"
#include <stack>
#include <string>
#include <vector>

namespace lcc {

class Program;
class Token;

class Instance {
public:
    explicit Instance(Program& prg);

    void run(const std::string& input);

    Token get_pc_token();

    MultiStack::ValStack& get_current_stack() { return m_stack[cur_stack_key]; }

private:
    using TokenIterator = std::vector<Token>::iterator;
    Program* program;
    std::vector<TokenIterator> specifiers;
    TokenIterator cur_token;
    MultiStack m_stack;
    Value cur_stack_key;

    char current_char; // #CHAR#
    std::string current_word; // #WORD#
    std::string current_text; // #TEXT#
    size_t input_length; // #INPUT_LENGTH#

    Value get_variable(std::string const&);

    void execute_specifier(TokenIterator& spec_it);
    void execute_command(TokenIterator&);
    void execute_conditional(TokenIterator&);
};

} // namespace lcc

#endif // LCC_INSTANCE_HPP