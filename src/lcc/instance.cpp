#include "lcc/instance.hpp"
#include "lcc/exceptions.hpp"
#include "lcc/program.hpp"
#include "lcc/token.hpp"

#include <cmath>
#include <iostream>

namespace lcc {

Instance::Instance(Program& prg) : program(&prg), cur_stack_key(Value(0)) {
    for (auto it = prg.tokens.begin(); it < prg.tokens.end(); it++)
        if (it->get_type() & TokenType::specifier)
            specifiers.emplace_back(it);

    m_stack.create_stack(cur_stack_key);
}

void Instance::run(const std::string& input) {
    for (auto it : specifiers)
        if (it->get_type() == TokenType::s_specifier)
            execute_specifier(it);
    for (char c : input) {
        current_char = c;
        current_text += c;
        for (auto it : specifiers)
            if (it->get_type() == TokenType::c_specifier)
                execute_specifier(it);
    }
    for (auto it : specifiers)
        if (it->get_type() == TokenType::e_specifier)
            execute_specifier(it);
}
void Instance::execute_specifier(TokenIterator& spec_it) {
    spec_it++; // Skip specifier token
    while (!(spec_it->get_type() & TokenType::block_ender)) { execute_command(spec_it); }
}
void Instance::execute_conditional(TokenIterator& cond_it) {
    cond_it++; // Skip conditional token
    while (!(cond_it->get_type() & TokenType::block_ender)) { execute_command(cond_it); }
}
void Instance::execute_command(TokenIterator& t) {
    cur_token = t;
    switch (t->get_type()) {
        case TokenType::s_specifier:
        case TokenType::c_specifier:
        case TokenType::w_specifier:
        case TokenType::e_specifier:
            // Handled by execute_specifier
            throw std::runtime_error("Internal runtime execution error, Unexpected token");

        case TokenType::equal_conditional:
        case TokenType::greater_conditional: execute_conditional(t); return;

        case TokenType::exit_parent:
        case TokenType::exit_specifier:
        case TokenType::exit_program:
        case TokenType::end_code_block:
            // Handled by execute_specifier or execute_conditional
            throw std::runtime_error("Internal runtime execution error, unexpected token");

        case TokenType::integer: get_current_stack().emplace(t->get_value_as<int>()); break;

        case TokenType::string_literal:
            get_current_stack().emplace(t->get_value_as<std::string>());
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

        case TokenType::multiply: {
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
                get_current_stack().emplace(val1.get<float>() * val2.get<float>());
            else if (val1.get_value_type() == ValueType::integer &&
                     val2.get_value_type() == ValueType::decimal)
                get_current_stack().emplace(val1.get<int>() * val2.get<float>());
            else if (val1.get_value_type() == ValueType::decimal &&
                     val2.get_value_type() == ValueType::integer)
                get_current_stack().emplace(val1.get<float>() * val2.get<int>());
            else if (val1.get_value_type() == ValueType::integer &&
                     val2.get_value_type() == ValueType::integer)
                get_current_stack().emplace(val1.get<int>() * val2.get<int>());
            break;
        }

        case TokenType::power: {
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
                get_current_stack().emplace(std::pow(val2.get<float>(), val1.get<float>()));
            else if (val2.get_value_type() == ValueType::integer &&
                     val1.get_value_type() == ValueType::decimal)
                get_current_stack().emplace(std::pow((float)val2.get<int>(), val1.get<float>()));
            else if (val2.get_value_type() == ValueType::decimal &&
                     val1.get_value_type() == ValueType::integer)
                get_current_stack().emplace(std::pow(val2.get<float>(), (float)val1.get<int>()));
            else if (val2.get_value_type() == ValueType::integer &&
                     val1.get_value_type() == ValueType::integer)
                get_current_stack().emplace(
                    std::pow((float)val2.get<int>(), (float)val1.get<int>()));
            break;
        }

        case TokenType::duplicate:
            if (get_current_stack().empty())
                throw NotEnoughStackItemsError(*this);
            get_current_stack().emplace(get_current_stack().top());
            break;

        case TokenType::chr: {
            if (get_current_stack().empty())
                throw NotEnoughStackItemsError(*this);
            if (get_current_stack().top().get_value_type() != ValueType::integer)
                throw ValueTypeError(*this, ValueType::integer);
            int chr = get_current_stack().top().get<int>();
            if (!isascii(chr)) {
                std::string err = "Invalid ASCII character code given to 'c': " +
                                  std::to_string(chr);
                throw std::runtime_error(err);
            }
            get_current_stack().emplace(std::string(1, (char)chr));
            break;
        }

        case TokenType::ord: {
            if (get_current_stack().empty())
                throw NotEnoughStackItemsError(*this);
            if (get_current_stack().top().get_value_type() != ValueType::integer)
                throw ValueTypeError(*this, ValueType::integer);
            auto str = get_current_stack().top().get<std::string>();
            if (str.size() != 1)
                throw std::runtime_error(
                    "'o' token requires a single character as the last stack value");
            get_current_stack().emplace((int)str[0]);
            break;
        }

        case TokenType::change_stack:
        case TokenType::move_val:
        case TokenType::jump_to:
        default: throw std::runtime_error("Not implemented TokenType");

        case TokenType::specifier:
        case TokenType::block_starter:
        case TokenType::block_ender: throw std::runtime_error("Invalid token. Broken build?");
    }
    t++;
}
Token Instance::get_pc_token() { return *cur_token; }

} // namespace lcc