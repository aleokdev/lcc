#include "lcc/instance.hpp"
#include "lcc/exceptions.hpp"
#include "lcc/program.hpp"
#include "lcc/token.hpp"

#include <iostream>

namespace lcc {

Instance::Instance(Program& prg) : program(&prg), cur_stack_key(Value(0)) {
    for (auto& t : prg.tokens)
        if ((size_t)t.get_type() & (size_t)TokenType::specifier)
            specifiers.emplace_back(&t);

    m_stack.create_stack(cur_stack_key);
}

void Instance::run(const std::string& input) {
    for (auto it = specifiers.begin(); it < specifiers.end(); it++) { execute_specifier(it); }
}
void Instance::execute_specifier(std::vector<Token*>::iterator& spec_it) {
    while ((*spec_it)->get_type() ^ TokenType::block_ender) {
        execute_command(spec_it);
        spec_it++;
    }
}
void Instance::execute_conditional(std::vector<Token*>::iterator& cond_it) {

    while ((*cond_it)->get_type() ^ TokenType::block_ender) {
        execute_command(cond_it);
        cond_it++;
    }
}
void Instance::execute_command(std::vector<lcc::Token*>::iterator& t) {
    switch ((*t)->get_type()) {
        case TokenType::s_specifier:
        case TokenType::c_specifier:
        case TokenType::w_specifier:
        case TokenType::e_specifier:
            // Handled by execute_specifier
            throw std::runtime_error("Unexpected token");

        case TokenType::equal_conditional:
        case TokenType::greater_conditional: execute_conditional(t); return;

        case TokenType::exit_parent:
        case TokenType::exit_specifier:
        case TokenType::exit_program:
        case TokenType::end_code_block:
            // Handled by execute_specifier or execute_conditional
            throw std::runtime_error("Internal runtime execution error, unexpected token");

        case TokenType::integer: get_current_stack().emplace((*t)->get_value_as<int>()); break;

        case TokenType::string_literal:
            get_current_stack().emplace((*t)->get_value_as<std::string>());
            break;

        case TokenType::char_expr:
            get_current_stack().emplace(Value(std::string(1, current_char)));
            break;

        case TokenType::word_expr: {
            std::string str = current_word;
            get_current_stack().emplace(Value(std::move(str)));
            break;
        }

        case TokenType::text_expr: {
            std::string str = current_text;
            get_current_stack().emplace(Value(std::move(str)));
            break;
        }

        case TokenType::input_length_expr:
            get_current_stack().emplace(Value((int)input_length));
            break;

        case TokenType::stack_length_expr:
            get_current_stack().emplace(Value((int)get_current_stack().size()));
            break;

        case TokenType::push_to_stdout:
            if (get_current_stack().empty())
                throw NotEnoughStackItemsError(*this);
            std::cout << get_current_stack().top().as_string();
            get_current_stack().pop();
            break;

        case TokenType::push_all_to_stdout:
            while (!get_current_stack().empty()) {
                std::cout << get_current_stack().top().as_string();
                get_current_stack().pop();
            }
            break;

        case TokenType::pop:
            if (get_current_stack().empty())
                throw NotEnoughStackItemsError(*this);
            get_current_stack().pop();
            break;

        case TokenType::clear:
            while (!get_current_stack().empty()) get_current_stack().pop();
            break;

        case TokenType::plus: {
            if (get_current_stack().size() < 2)
                throw NotEnoughStackItemsError(*this);

            const Value val1 = get_current_stack().top();
            get_current_stack().pop();
            if (val1.get_value_type() != ValueType::integer &&
                val1.get_value_type() != ValueType::decimal)
                throw ValueTypeError(*this, ValueType::number);

            const Value val2 = get_current_stack().top();
            get_current_stack().pop();
            if (val2.get_value_type() != ValueType::integer &&
                val2.get_value_type() != ValueType::decimal)
                throw ValueTypeError(*this, ValueType::number);

            if (val1.get_value_type() == ValueType::decimal &&
                val2.get_value_type() == ValueType::decimal)
                get_current_stack().emplace(val1.get<float>() + val2.get<float>());
            else if (val1.get_value_type() == ValueType::integer &&
                     val2.get_value_type() == ValueType::decimal)
                get_current_stack().emplace(val1.get<int>() + val2.get<float>());
            else if (val1.get_value_type() == ValueType::decimal &&
                     val2.get_value_type() == ValueType::integer)
                get_current_stack().emplace(val1.get<float>() + val2.get<int>());
            else if (val1.get_value_type() == ValueType::integer &&
                     val2.get_value_type() == ValueType::integer)
                get_current_stack().emplace(val1.get<int>() + val2.get<int>());
            break;
        }

        case TokenType::minus: {
            if (get_current_stack().size() < 2)
                throw NotEnoughStackItemsError(*this);

            const Value val1 = get_current_stack().top();
            get_current_stack().pop();
            if (val1.get_value_type() != ValueType::integer &&
                val1.get_value_type() != ValueType::decimal)
                throw ValueTypeError(*this, ValueType::number);

            const Value val2 = get_current_stack().top();
            get_current_stack().pop();
            if (val2.get_value_type() != ValueType::integer &&
                val2.get_value_type() != ValueType::decimal)
                throw ValueTypeError(*this, ValueType::number);

            if (val2.get_value_type() == ValueType::decimal &&
                val1.get_value_type() == ValueType::decimal)
                get_current_stack().emplace(val2.get<float>() - val1.get<float>());
            else if (val2.get_value_type() == ValueType::integer &&
                     val1.get_value_type() == ValueType::decimal)
                get_current_stack().emplace(val2.get<int>() - val1.get<float>());
            else if (val2.get_value_type() == ValueType::decimal &&
                     val1.get_value_type() == ValueType::integer)
                get_current_stack().emplace(val2.get<float>() - val1.get<int>());
            else if (val2.get_value_type() == ValueType::integer &&
                     val1.get_value_type() == ValueType::integer)
                get_current_stack().emplace(val2.get<int>() - val1.get<int>());
            break;
        }

        case TokenType::ord:
        case TokenType::chr:
        case TokenType::duplicate:
        case TokenType::change_stack:
        case TokenType::move_val:
        case TokenType::jump_to:
        case TokenType::cond_less:
        case TokenType::cond_greater:
        case TokenType::cond_eq_less:
        case TokenType::cond_eq_greater:
        case TokenType::cond_equal:
        default: throw std::runtime_error("Not implemented TokenType");

        case TokenType::specifier:
        case TokenType::block_starter:
        case TokenType::block_ender: throw std::runtime_error("Invalid token. Broken build?");
    }
}
Token Instance::get_pc_token() { return **cur_token; }

} // namespace lcc