#include <iostream>
#include "AST.h"
#include "Emulator.h"

extern syntax_tree::AST analize(int argc, char* argv[]);

int main(int argc, char* argv[])
{   
    syntax_tree::AST ast = analize(argc, argv);
    ast.print();

    Emulator* e = new Emulator();
    syntax_tree::AST result = e->eval(std::move(ast));
    std::cout << "\nResult tree:\n";
    result.print(true);

    return 0;
}