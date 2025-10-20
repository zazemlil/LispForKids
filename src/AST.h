#pragma once
#include <memory>
#include <vector>
#include <string>
#include <iostream>

namespace syntax_tree {

class ASTNode {
private:
    std::string node_type;
    std::vector<std::shared_ptr<ASTNode>> statements;

public:
    ASTNode() {}
    ASTNode(std::string t) : node_type(t) {}
    
    virtual ~ASTNode() = default;

    void addStatement(std::shared_ptr<ASTNode> stmt) {
        statements.push_back(stmt);
    }

    void addStatements(std::vector<std::shared_ptr<ASTNode>> new_statements) {
        this->statements.insert(
            std::end(this->statements),
            std::begin(new_statements),
            std::end(new_statements)
        );
    }

    void setStatements(std::vector<std::shared_ptr<ASTNode>> new_statements) {
        this->statements = new_statements;
    }
    
    size_t getStatementCount() const { 
        return statements.size(); 
    }

    std::shared_ptr<ASTNode> getStatement(size_t index) const { 
        return statements.at(index);
    }

    virtual void printValue() const {
        std::cout << node_type << " ";
    }

    void print(int indent = 0) const {
        std::string indentStr = ""; // "|"
        for (int i = 0; i < indent-1; i++) {indentStr += "    ";} // "    |"
        
        std::cout << indentStr << ""; // "--- "
        this->printValue();
        std::cout << '\n';
        
        for (const auto& stmt : statements) {
            stmt->print(indent + 2);
        }
    }
};


// unary
class QuoteNode : public ASTNode {
public:
    QuoteNode(std::string t) : ASTNode(t) {}
};

class CarNode : public ASTNode {
public:
    CarNode(std::string t) : ASTNode(t) {}
};

class CdrNode : public ASTNode {
public:
    CdrNode(std::string t) : ASTNode(t) {}
};

class AtomNode : public ASTNode {
public:
    AtomNode(std::string t) : ASTNode(t) {}
};


// binary
class AddNode : public ASTNode {
public:
    AddNode(std::string t) : ASTNode(t) {}
};

class SubNode : public ASTNode {
public:
    SubNode(std::string t) : ASTNode(t) {}
};

class MulNode : public ASTNode {
public:
    MulNode(std::string t) : ASTNode(t) {}
};

class DiveNode : public ASTNode {
public:
    DiveNode(std::string t) : ASTNode(t) {}
};

class RemNode : public ASTNode {
public:
    RemNode(std::string t) : ASTNode(t) {}
};

class LeNode : public ASTNode {
public:
    LeNode(std::string t) : ASTNode(t) {}
};

class ConsNode : public ASTNode {
private:
    std::vector<std::shared_ptr<ASTNode>> value;

public:
    std::vector<std::shared_ptr<ASTNode>>& getValue() {
        return value;
    }

    void addToValue(std::vector<std::shared_ptr<ASTNode>> value) {
        this->value.insert(
            std::end(this->value),
            std::begin(value),
            std::end(value)
        );
    }

    void printValue() const override {
        std::cout << '(';
        for (const auto& v : value) {
            v->printValue();
            std::cout << ' ';
        }
        std::cout << ")\n";
    }

    ConsNode(std::string t) : ASTNode(t) {}
};

class EqualNode : public ASTNode {
public:
    EqualNode(std::string t) : ASTNode(t) {}
};


// ternary
class CondNode : public ASTNode {
public:
    CondNode(std::string t) : ASTNode(t) {}
};


// other Nodes
class FCallNode : public ASTNode {
public:
    FCallNode(std::string t) : ASTNode(t) {}
}; // maybe needs to fix AST creation in parser

class LambdaNode : public ASTNode {
public:
    LambdaNode(std::string t) : ASTNode(t) {}
};

class LetNode : public ASTNode {
public:
    LetNode(std::string t) : ASTNode(t) {}
};

class LetrecNode : public ASTNode {
public:
    LetrecNode(std::string t) : ASTNode(t) {}
};


class LiteralInt : public ASTNode {
    int value;

public:
    void printValue() const override {
        std::cout << value;
    }

    int getValue() {
        return value;
    }

    LiteralInt(std::string t, int v) : ASTNode(t), value(v) {}
};

class LiteralBool : public ASTNode {
    bool value;

public:
    void printValue() const override {
        if (value) {
            std::cout << "TRUE";
        }
        else {
            std::cout << "FALSE";
        }
    }

    bool getValue() {
        return value;
    }

    LiteralBool(std::string t, bool v) : ASTNode(t), value(v) {}
};

class LiteralNil : public ASTNode {
public:
    LiteralNil(std::string t) : ASTNode(t) {}
};

class Identifier : public ASTNode {
    std::string value;

public:
    void printValue() const override {
        std::cout << value;
    }

    Identifier(std::string t, std::string v) : ASTNode(t), value(v) {}
};

class AST {
private:
    std::shared_ptr<ASTNode> root = nullptr;

public:
    AST() {}
    AST(std::shared_ptr<ASTNode> rootNode) : root(rootNode) {}

    std::shared_ptr<ASTNode> getRoot() {
        return root;
    }

    bool isEmpty() const {
        return root == nullptr;
    }

    void print() const {
        if (root) {
            root->print(0);
        } else {
            std::cout << "AST is empty.\n";
        }
    }
};

};