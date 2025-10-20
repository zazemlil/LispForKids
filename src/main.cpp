#include <iostream>
#include "AST.h"
#include "Emulator.h"

extern syntax_tree::AST analize(int argc, char* argv[]);

int main(int argc, char* argv[])
{   
    syntax_tree::AST ast = analize(argc, argv);
    ast.print();

    Emulator* e = new Emulator();
    std::cout << "\nResult tree:\n";
    syntax_tree::AST result = e->eval(std::move(ast));
    result.print(false);
    std::cout << "\n";

    return 0;
}