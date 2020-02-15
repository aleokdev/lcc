#ifndef LCC_EXCEPTIONS_HPP__
#define LCC_EXCEPTIONS_HPP__

#include <cstdio>
#include <exception>
#include <memory>
#include <string>
#include <utility>

#include "value.hpp"

namespace lcc {

class Program;
class Instance;

class SyntaxError : public std::exception {
private:
    Program* prg;
    std::string message;

public:
    explicit SyntaxError(Program& _prg, std::string _what) : prg(&_prg), message(_what) {}

    [[nodiscard]] const char* what() const noexcept override;
};

class UnexpectedEOFError : public std::exception {
private:
    Program* prg;

public:
    explicit UnexpectedEOFError(Program& _prg) : prg(&_prg) {}

    [[nodiscard]] const char* what() const noexcept override;
};

class UnidentifiedTokenError : public std::exception {
private:
    Program* prg;

public:
    explicit UnidentifiedTokenError(Program& _prg) : prg(&_prg) {}

    [[nodiscard]] const char* what() const noexcept override;
};

// Runtime exceptions

class NotEnoughStackItemsError : public std::exception {
private:
    Instance* instance;

public:
    explicit NotEnoughStackItemsError(Instance& _i) : instance(&_i) {}

    [[nodiscard]] const char* what() const noexcept override;
};

class ValueTypeError : public std::exception {
private:
    Instance* instance;
    ValueType expected_type;

public:
    explicit ValueTypeError(Instance& _i, ValueType expected) :
        instance(&_i), expected_type(expected) {}

    [[nodiscard]] const char* what() const noexcept override;
};

} // namespace lcc
#endif