#pragma once
#include <memory>
#include <vector>
#include <string>
#include <iostream>

namespace syntax_tree {

class ASTNode {
private:
    std::string node_type;
    std::vector<std::unique_ptr<ASTNode>> statements;

public:
    ASTNode() {}
    ASTNode(std::string t) : node_type(t) {}
    
    virtual ~ASTNode() = default;

    void addStatement(std::unique_ptr<ASTNode> stmt) {
        statements.push_back(std::move(stmt));
    }

    void addStatements(std::vector<std::unique_ptr<ASTNode>> statements) {
        this->statements.insert(
            std::end(this->statements),
            std::make_move_iterator(std::begin(statements)),
            std::make_move_iterator(std::end(statements))
        );
    }

    void setStatements(std::vector<std::unique_ptr<ASTNode>> statements) {
        this->statements = std::move(statements);
    }
    
    size_t getStatementCount() const { 
        return statements.size(); 
    }

    ASTNode* getStatement(size_t index) const { 
        return statements.at(index).get(); 
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

class LiteralInt : public ASTNode {
    int value;

public:
    void printValue() const override {
        std::cout << value;
    }

    LiteralInt(std::string t, int v) : ASTNode(t), value(v) {}
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
    std::unique_ptr<ASTNode> root = nullptr;

public:
    AST() {}
    AST(std::unique_ptr<ASTNode> rootNode) : root(std::move(rootNode)) {}

    bool isEmpty() {
        if (root == nullptr) return true;
        return false;
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