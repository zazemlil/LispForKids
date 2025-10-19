#pragma once

#include <vector>
#include "AST.h"

typedef std::vector<std::vector<std::shared_ptr<syntax_tree::ASTNode>>> Matrix;

class Emulator {
private:
    std::shared_ptr<syntax_tree::ASTNode> eval(std::shared_ptr<syntax_tree::ASTNode> e, Matrix& n, Matrix& v);
    std::shared_ptr<syntax_tree::ASTNode> evalLiteral(std::shared_ptr<syntax_tree::LiteralInt> lit, Matrix& n, Matrix& v);

public:
    syntax_tree::AST eval(syntax_tree::AST ast);
};