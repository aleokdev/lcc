#ifndef LCC_PROGRAM_HPP__
#define LCC_PROGRAM_HPP__

#include <filesystem>
#include <string_view>
#include <vector>

namespace lcc {
class Program {
public:
    Program();

    static Program from_string(std::string_view);
    static Program from_file(std::filesystem::path)

private:
    std::vector<Token> tokens;
};
} // namespace lcc

#endif