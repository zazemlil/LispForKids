#pragma once
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

namespace syntax_tree {

class ASTNode {
    std::string node_type;
    std::vector<std::shared_ptr<ASTNode>> statements;

public:
    ASTNode() {}
    ASTNode(std::string t) : node_type(t) {}
    
    virtual ~ASTNode() = default;

    std::string getNodeType() const { return node_type; }
    size_t getStatementCount() const { return statements.size(); }

    std::shared_ptr<ASTNode>& getStatement(size_t index) { return statements.at(index); }
    void addStatement(std::shared_ptr<ASTNode> stmt) { statements.push_back(stmt); }

    void setStatements(std::vector<std::shared_ptr<ASTNode>>& new_statements) { this->statements = new_statements; }
    std::vector<std::shared_ptr<ASTNode>>& getStatements() { return statements; }
    
    void addStatements(std::vector<std::shared_ptr<ASTNode>>& new_statements) {
        this->statements.insert(
            std::end(this->statements),
            std::begin(new_statements),
            std::end(new_statements)
        );
    }

    virtual void printValue(std::ostream& os = std::cout) const { os << node_type; }

    void printRec(int deep, int maxDeep, int indent = 0) const {
        std::string indentStr = ""; 
        for (int i = 0; i < indent-1; i++) {indentStr += "    ";} 
        
        std::cout << indentStr << "";
        this->printValue();
        std::cout << '\n';
        
        if (deep <= maxDeep)
        for (const auto& stmt : statements) {
            stmt->printRec(deep+1, maxDeep, indent + 2);
        }
    }

    void print(int indent = 0, std::ostream& os = std::cout) const {
        std::string indentStr = ""; 
        for (int i = 0; i < indent-1; i++) {indentStr += "    ";} 
        
        os << indentStr << "";
        this->printValue(os);
        os << '\n';
        
        for (const auto& stmt : statements) {
            stmt->print(indent + 2, os);
        }
    }

    virtual void printFlat(int depth = 0, std::ostream& os = std::cout) {
        if (!statements.empty()) {
            os << "(";
        }
        
        printValue(os);
        
        if (!statements.empty()) {
            for (const auto& stmt : statements) {
                os << " ";
                stmt->printFlat(depth + 1, os);
            }
            os << ")";
        }
    }
};


class AST {
private:
    std::shared_ptr<ASTNode> root = nullptr;

public:
    AST() {}
    AST(std::shared_ptr<ASTNode> rootNode) : root(rootNode) {}

    std::shared_ptr<ASTNode> getRoot() { return root; }
    bool isEmpty() const { return root == nullptr; }

    void print(bool flat = false, std::ostream& os = std::cout) const {
        if (root) {
            if (!flat) root->print(0);
            else root->printFlat(0, os);
            std::cout << "\n";
        } else {
            std::cout << "AST is empty.\n\n";
        }
    }
};


// unary
class QuoteNode : public ASTNode { public: QuoteNode(std::string t) : ASTNode(t) {} };
class CarNode : public ASTNode { public: CarNode(std::string t) : ASTNode(t) {} };
class CdrNode : public ASTNode { public: CdrNode(std::string t) : ASTNode(t) {} };
class AtomNode : public ASTNode { public: AtomNode(std::string t) : ASTNode(t) {} };
class LiteralNode : public ASTNode { public: LiteralNode(std::string t) : ASTNode(t) {} };

// binary
class AddNode : public ASTNode { public: AddNode(std::string t) : ASTNode(t) {} };
class SubNode : public ASTNode { public: SubNode(std::string t) : ASTNode(t) {} };
class MulNode : public ASTNode { public: MulNode(std::string t) : ASTNode(t) {} };
class DiveNode : public ASTNode { public: DiveNode(std::string t) : ASTNode(t) {} };
class RemNode : public ASTNode { public: RemNode(std::string t) : ASTNode(t) {} };
class LeNode : public ASTNode { public: LeNode(std::string t) : ASTNode(t) {} };
class ConsNode : public ASTNode { public: ConsNode(std::string t) : ASTNode(t) {} };
class EqualNode : public ASTNode { public: EqualNode(std::string t) : ASTNode(t) {} };

// ternary
class CondNode : public ASTNode { public: CondNode(std::string t) : ASTNode(t) {} };

// other Nodes
class LambdaNode : public ASTNode { public: LambdaNode(std::string t) : ASTNode(t) {} };
class FuncClosureNode : public ASTNode { 
public: 
    FuncClosureNode(std::string t) : ASTNode(t) {} 
    void printFlat(int depth = 0, std::ostream& os = std::cout) override {
        os << "(";
        if (!getStatements().empty()) {
            for (const auto& stmt : getStatements()) {
                os << " ";
                stmt->printFlat(depth, os);
            }
        }
        os << ")";
    }
};
class LetNode : public ASTNode { public: LetNode(std::string t) : ASTNode(t) {} };
class LetrecNode : public ASTNode { public: LetrecNode(std::string t) : ASTNode(t) {} };


class LiteralInt : public ASTNode {
    int value;
public:
    void printValue(std::ostream& os = std::cout) const override { os << value; }
    int getValue() { return value; }
    LiteralInt(std::string t, int v) : ASTNode(t), value(v) {}
};

class LiteralBool : public ASTNode {
    bool value;
public:
    void printValue(std::ostream& os = std::cout) const override {
        if (value) { os << "TRUE"; }
        else { os << "FALSE"; }
    }
    bool getValue() { return value; }
    LiteralBool(std::string t, bool v) : ASTNode(t), value(v) {}
};

class ListNode : public ASTNode { 
public: 
    ListNode(std::string t) : ASTNode(t) {}
    void printFlat(int depth = 0, std::ostream& os = std::cout) override {
        os << "(";
        if (!getStatements().empty()) {
            for (const auto& stmt : getStatements()) {
                os << " ";
                stmt->printFlat(depth, os);
            }
        }
        os << ")";
    }
};

class LiteralNil : public ASTNode { public: LiteralNil(std::string t) : ASTNode(t) {} };

class Identifier : public ASTNode {
    std::string value;
public:
    void printValue(std::ostream& os = std::cout) const override { os << value; }
    std::string getValue() { return value; }
    Identifier(std::string t, std::string v) : ASTNode(t), value(v) {}
};


};