#include <iostream>
#include "AST.h"
#include "Emulator.h"

extern syntax_tree::AST analize(int argc, char* argv[]);

int main(int argc, char* argv[])
{   
    syntax_tree::AST ast = analize(argc, argv);
    std::cout << "-----------------------------\n";
    std::cout << "----Abstract syntax tree:----\n";
    std::cout << "-----------------------------\n";
    ast.print();

    Emulator* e = new Emulator();
    syntax_tree::AST result;
    try {
        result = e->eval(ast);
    }
    catch(const std::exception& e) {
        std::cerr << "Evaluation error: `" << e.what() << "`\n\n";
    }
    std::cout << "-----------------------------\n";
    std::cout << "--------Evaluated AST:-------\n";
    std::cout << "-----------------------------\n";
    result.print(false);
    std::cout << "-----------------------------\n";
    std::cout << "------Evaluated flat AST:----\n";
    std::cout << "-----------------------------\n";
    result.print(true);
    
    std::cout << "\n";

    return 0;
}