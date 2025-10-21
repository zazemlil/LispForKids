#pragma once

#include <vector>
#include "AST.h"

typedef std::vector<std::vector<std::shared_ptr<syntax_tree::ASTNode>>> Matrix;
typedef std::shared_ptr<syntax_tree::ASTNode> Node;
typedef std::shared_ptr<syntax_tree::ListNode> ListNode;
typedef std::shared_ptr<syntax_tree::LiteralInt> LiteralInt;
typedef std::shared_ptr<syntax_tree::LiteralNil> LiteralNil;
typedef std::shared_ptr<syntax_tree::LiteralBool> LiteralBool;
typedef std::shared_ptr<syntax_tree::Identifier> Identifier;
typedef std::shared_ptr<syntax_tree::QuoteNode> QuoteNode;
typedef std::shared_ptr<syntax_tree::CarNode> CarNode;
typedef std::shared_ptr<syntax_tree::CdrNode> CdrNode;
typedef std::shared_ptr<syntax_tree::AtomNode> AtomNode;
typedef std::shared_ptr<syntax_tree::AddNode> AddNode;
typedef std::shared_ptr<syntax_tree::SubNode> SubNode;
typedef std::shared_ptr<syntax_tree::MulNode> MulNode;
typedef std::shared_ptr<syntax_tree::DiveNode> DiveNode;
typedef std::shared_ptr<syntax_tree::RemNode> RemNode;
typedef std::shared_ptr<syntax_tree::LeNode> LeNode;
typedef std::shared_ptr<syntax_tree::ConsNode> ConsNode;
typedef std::shared_ptr<syntax_tree::EqualNode> EqualNode;
typedef std::shared_ptr<syntax_tree::CondNode> CondNode;
typedef std::shared_ptr<syntax_tree::LambdaNode> LambdaNode;
typedef std::shared_ptr<syntax_tree::FuncClosureNode> FuncClosureNode;

class Emulator {
private:
    Node eval(Node e, Matrix& n, Matrix& v);

    LiteralInt evalLiteralInt(LiteralInt litInt, Matrix& n, Matrix& v);
    LiteralNil evalLiteralNil(LiteralNil litNil, Matrix& n, Matrix& v);
    LiteralBool evalLiteralBool(LiteralBool litBool, Matrix& n, Matrix& v);
    Node evalIdentifier(Identifier id, Matrix& n, Matrix& v);

    // unary
    Node evalQuoteNode(QuoteNode quote, Matrix& n, Matrix& v);
    Node evalCarNode(CarNode car, Matrix& n, Matrix& v);
    Node evalCdrNode(CdrNode cdr, Matrix& n, Matrix& v);
    Node evalAtomNode(AtomNode atom, Matrix& n, Matrix& v);

    // binary
    LiteralInt evalAddNode(AddNode add, Matrix& n, Matrix& v);
    LiteralInt evalSubNode(SubNode sub, Matrix& n, Matrix& v);
    LiteralInt evalMulNode(MulNode mul, Matrix& n, Matrix& v);
    LiteralInt evalDiveNode(DiveNode dive, Matrix& n, Matrix& v);
    LiteralInt evalRemNode(RemNode rem, Matrix& n, Matrix& v);
    LiteralBool evalLeNode(LeNode le, Matrix& n, Matrix& v);
    ListNode evalConsNode(ConsNode cons, Matrix& n, Matrix& v);
    LiteralBool evalEqualNode(EqualNode equal, Matrix& n, Matrix& v);

    // ternary
    Node evalCondNode(CondNode cond, Matrix& n, Matrix& v);

    //other
    FuncClosureNode evalLambdaNode(LambdaNode lambda, Matrix& n, Matrix& v);
    Node evalFuncCall(Node func, Matrix& n, Matrix& v);

    //auxiliary functions
    Node matrixToListNode(const Matrix& matrix);
    Node assoc(Identifier id, Matrix& n, Matrix& v);

public:
    syntax_tree::AST eval(syntax_tree::AST ast);
};