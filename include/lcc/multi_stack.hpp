#ifndef LCC_MULTI_STACK_HPP
#define LCC_MULTI_STACK_HPP

#include <stack>
#include <vector>
#include <exception>
#include "lcc/value.hpp"

namespace lcc{

// A multidimensional stack. Stores stacks and associates them with a value.
class MultiStack{
public:
    using ValStack = std::stack<Value>;
    ValStack& create_stack(Value const& key) { return values.emplace_back(key, ValStack{}).second; }

    ValStack& operator[](Value const& key) {
        for(auto& [s_key, stack] : values)
            if (s_key == key)
                return stack;
        throw std::runtime_error("Could not find key in multistack.");
    }

    [[nodiscard]] std::vector<std::pair<Value, ValStack>> const& get_values() { return values; }

private:
    std::vector<std::pair<Value, ValStack>> values;
};

}

#endif // LCC_MULTI_STACK_HPP
