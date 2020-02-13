#ifndef LCC_EXCEPTIONS_HPP__
#define LCC_EXCEPTIONS_HPP__

#include <cstdio>
#include <exception>
#include <memory>
#include <string>
#include <utility>

namespace lcc {

class Program;

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
} // namespace lcc
#endif