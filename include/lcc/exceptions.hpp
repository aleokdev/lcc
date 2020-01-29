#ifndef LCC_EXCEPTIONS_HPP__
#define LCC_EXCEPTIONS_HPP__

#include <exception>
#include <memory>
#include <stdio.h>
#include <string>

namespace lcc {

class SyntaxError : public std::exception {
private:
    size_t character;
    std::string token;

public:
    SyntaxError(size_t _character, std::string _token) :
        character(_character), token(_token) {}

    const char* what() const override {
        char buf[64];
        sprintf_s(buf, 64, "Unrecognized token at character %zu: %s", character, token.c_str());
        return buf;
    }
};
} // namespace lcc
#endif