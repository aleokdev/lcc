//
// Created by aleok on 15/2/20.
//

#ifndef LCC_VALUE_HPP
#define LCC_VALUE_HPP

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>

namespace lcc {

enum class ValueType {
    string,
    integer,
    decimal,
    boolean,
    number = 128 // Special case ONLY used for formatting exceptions.
};
inline std::unordered_map<ValueType, std::string_view> value_type_names = {
    {ValueType::string, "string"},
    {ValueType::integer, "integer"},
    {ValueType::decimal, "decimal"},
    {ValueType::boolean, "boolean"}};

class Value {
public:
    explicit Value(std::string&& val) { value = std::move(val); }
    explicit Value(int val) { value = val; }
    explicit Value(float val) { value = val; }
    explicit Value(bool val) { value = val; }

    template<typename T> T get() const { return std::get<T>(value); }

    [[nodiscard]] std::string as_string() const {
        switch (value.index()) {
            case 0: // std::string
                return std::get<std::string>(value);
            case 1: // int
                return std::to_string(std::get<int>(value));
            case 2: // float
                return std::to_string(std::get<float>(value));
            case 3: // bool
                return std::get<bool>(value) ? "true" : "false";
            default: // wut
                throw std::runtime_error("Not implemented value as_string");
        }
    };

    [[nodiscard]] ValueType get_value_type() const { return (ValueType)value.index(); }

    bool operator==(const Value& other) { return other.value == value; }

private:
    std::variant<std::string, int, float, bool> value;
};

} // namespace lcc

#endif // LCC_VALUE_HPP
