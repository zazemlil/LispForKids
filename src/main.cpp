#include <iostream>
#include "AST.h"
#include "Emulator.h"

extern syntax_tree::AST analize(int argc, char* argv[]);

int main(int argc, char* argv[])
{   
    syntax_tree::AST ast = analize(argc, argv);

    if (argc < 3) {
        std::cout << "-----------------------------\n";
        std::cout << "----Abstract syntax tree:----\n";
        std::cout << "-----------------------------\n";
        ast.print();
    }

    Emulator* e = new Emulator();
    syntax_tree::AST result;
    try {
        result = e->eval(ast);
        std::cout << "Evaluation success.\n";
    }
    catch(const std::exception& e) {
        std::cerr << "Evaluation error: `" << e.what() << "`\n\n";
    }

    if (argc < 3) {
        std::cout << "-----------------------------\n";
        std::cout << "--------Evaluated AST:-------\n";
        std::cout << "-----------------------------\n";
        result.print(false);
        std::cout << "-----------------------------\n";
        std::cout << "------Evaluated flat AST:----\n";
        std::cout << "-----------------------------\n";
        result.print(true);
        std::cout << "\n";
    }
    else {
        auto fileStream = std::make_unique<std::ofstream>(argv[2], std::ios::out | std::ios::trunc);
        if (fileStream->is_open()) {
            result.print(true, *fileStream);
            fileStream->close();
        }
        else {
            std::cerr << "The file is not open.\n";
        }
    }
    
    return 0;
}