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
    Program* program;
    std::vector<Token*> specifiers;
    std::vector<Token*>::iterator cur_token;
    MultiStack m_stack;
    Value cur_stack_key;

    char current_char; // #CHAR#
    std::string current_word; // #WORD#
    std::string current_text; // #TEXT#
    size_t input_length; // #INPUT_LENGTH#

    void execute_specifier(std::vector<Token*>::iterator& spec_it);
    void execute_command(std::vector<Token*>::iterator&);
    void execute_conditional(std::vector<Token*>::iterator&);
};

} // namespace lcc

#endif // LCC_INSTANCE_HPP