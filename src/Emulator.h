#pragma once

namespace syntax_tree {
    class AST;
};

class Emulator {
private:
    
public:
    syntax_tree::AST eval(syntax_tree::AST ast);
};