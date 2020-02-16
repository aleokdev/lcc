#include "lcc/program.hpp"
#include "lcc/instance.hpp"

#include <exception>
#include <iostream>

int main() {
    std::cout << "LCC Shell version " << LCC_VERSION << " running Linecrypt version " << LINECRYPT_VERSION << std::endl;
    std::cout << "By aleok (alexdevteam) 2019~2020, https://github.com/alexdevteam/lcc" << std::endl;
    std::cout << "Type 'exit' to exit." << std::endl;
    std::cout << "-------------------------------------------------------------------" << std::endl;
    for(;;)
    {
        std::cout << ">> ";
        std::string input;
        std::getline(std::cin, input);
        if(input == "exit")
            return 1;
        std::cout.flush();
        lcc::Program prg;
        bool success = false;
        try{
            prg = lcc::Program::from_string(input);
            success = true;
        }catch(std::exception const& e){
            std::cerr << "[FATAL] Error while tokenizing: " << e.what() << std::endl;
        }
        if (success) try{
            lcc::Instance(prg).run("");
        }catch(std::exception const& e){
            std::cerr << "[FATAL] Runtime error: " << e.what() << std::endl;
        }
        std::cerr.flush();
    }
}
