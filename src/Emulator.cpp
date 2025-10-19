#include "Emulator.h"
#include <iostream>

syntax_tree::AST Emulator::eval(syntax_tree::AST ast) {
    Matrix n;
    Matrix v;
    std::shared_ptr<syntax_tree::ASTNode> root = eval(ast.getRoot(), n, v);
    return syntax_tree::AST(root);
}

std::shared_ptr<syntax_tree::ASTNode> Emulator::eval(std::shared_ptr<syntax_tree::ASTNode> e, Matrix& n, Matrix& v) {
    if (auto lit = std::dynamic_pointer_cast<syntax_tree::LiteralInt>(e)) {
        return evalLiteral(lit, n, v);
    }
    else if (auto add = std::dynamic_pointer_cast<syntax_tree::AddNode>(e)) {
        // Pattern для AddNode
        //return evalAdd(add, n, v);
    }
    else if (auto ident = std::dynamic_pointer_cast<syntax_tree::Identifier>(e)) {
        // Pattern для Identifier
        //return evalIdentifier(ident, n, v);
    }
    
    throw std::runtime_error("Unknown node type");
}

std::shared_ptr<syntax_tree::ASTNode> Emulator::evalLiteral(std::shared_ptr<syntax_tree::LiteralInt> lit, Matrix& n, Matrix& v) {
    return lit;
}