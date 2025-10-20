#include "Emulator.h"
#include <iostream>

syntax_tree::AST Emulator::eval(syntax_tree::AST ast) {
    Matrix n;
    Matrix v;
    Node root = eval(ast.getRoot(), n, v);
    return syntax_tree::AST(root);
}

Node Emulator::eval(Node e, Matrix& n, Matrix& v) {
    if (auto litInt = std::dynamic_pointer_cast<syntax_tree::LiteralInt>(e)) {
        return evalLiteralInt(litInt, n, v);
    }
    if (auto litBool = std::dynamic_pointer_cast<syntax_tree::LiteralBool>(e)) {
        return evalLiteralBool(litBool, n, v);
    }
    if (auto litNil = std::dynamic_pointer_cast<syntax_tree::LiteralNil>(e)) {
        return evalLiteralNil(litNil, n, v);
    }
    else if (auto id = std::dynamic_pointer_cast<syntax_tree::Identifier>(e)) {
        return evalIdentifier(id, n, v);
    }
    else if (auto quote = std::dynamic_pointer_cast<syntax_tree::QuoteNode>(e)) {
        return evalQuoteNode(quote, n, v);
    }
    else if (auto car = std::dynamic_pointer_cast<syntax_tree::CarNode>(e)) {
        return evalCarNode(car, n, v);
    }
    else if (auto cdr = std::dynamic_pointer_cast<syntax_tree::CdrNode>(e)) {
        return evalCdrNode(cdr, n, v);
    }
    else if (auto atom = std::dynamic_pointer_cast<syntax_tree::AtomNode>(e)) {
        return evalAtomNode(atom, n, v);
    }
    else if (auto add = std::dynamic_pointer_cast<syntax_tree::AddNode>(e)) {
        return evalAddNode(add, n, v);
    }
    else if (auto sub = std::dynamic_pointer_cast<syntax_tree::SubNode>(e)) {
        return evalSubNode(sub, n, v);
    }
    else if (auto mul = std::dynamic_pointer_cast<syntax_tree::MulNode>(e)) {
        return evalMulNode(mul, n, v);
    }
    else if (auto dive = std::dynamic_pointer_cast<syntax_tree::DiveNode>(e)) {
        return evalDiveNode(dive, n, v);
    }
    else if (auto rem = std::dynamic_pointer_cast<syntax_tree::RemNode>(e)) {
        return evalRemNode(rem, n, v);
    }
    else if (auto le = std::dynamic_pointer_cast<syntax_tree::LeNode>(e)) {
        return evalLeNode(le, n, v);
    }
    else if (auto cons = std::dynamic_pointer_cast<syntax_tree::ConsNode>(e)) {
        return evalConsNode(cons, n, v);
    }
    else if (auto equal = std::dynamic_pointer_cast<syntax_tree::EqualNode>(e)) {
        return evalEqualNode(equal, n, v);
    }
    else if (auto cond = std::dynamic_pointer_cast<syntax_tree::CondNode>(e)) {
        return evalCondNode(cond, n, v);
    }

    throw std::runtime_error("Unknown node type");
}

LiteralInt Emulator::evalLiteralInt(LiteralInt litInt, Matrix& n, Matrix& v) {
    return litInt;
}

LiteralNil Emulator::evalLiteralNil(LiteralNil litNil, Matrix& n, Matrix& v) {
    return litNil;
}

LiteralBool Emulator::evalLiteralBool(LiteralBool litBool, Matrix& n, Matrix& v) {
    return litBool;
}

Identifier Emulator::evalIdentifier(Identifier id, Matrix& n, Matrix& v) {
    return id;
}

Node Emulator::evalQuoteNode(QuoteNode quote, Matrix& n, Matrix& v) {
    return quote->getStatement(0);
}

Node Emulator::evalCarNode(CarNode car, Matrix& n, Matrix& v) {
    auto c = eval(car->getStatement(0), n, v);
    
    if (auto nil = std::dynamic_pointer_cast<syntax_tree::LiteralNil>(c)) {
        return nil;
    } else if (c->getStatementCount() >= 1) {
        auto first = c->getStatement(0);
        if (auto cons = std::dynamic_pointer_cast<syntax_tree::ListNode>(c)) {
            return first;
        }
    }
    
    throw std::runtime_error("Car error!");
}

Node Emulator::evalCdrNode(CdrNode cdr, Matrix& n, Matrix& v) {
    auto c = eval(cdr->getStatement(0), n, v);
    
    if (auto nil = std::dynamic_pointer_cast<syntax_tree::LiteralNil>(c)) {
        return nil;
    } else if (c->getStatementCount() >= 2) {
        auto new_list = std::make_shared<syntax_tree::ListNode>("LIST");
        for (size_t i = 1; i < c->getStatementCount(); ++i) {
            new_list->addStatement(c->getStatement(i));
        }
        return new_list;
    } else if (c->getStatementCount() == 1) {
        return std::make_shared<syntax_tree::LiteralNil>("NIL");
    }
    
    throw std::runtime_error("Cdr error!");
}

Node Emulator::evalAtomNode(AtomNode atom, Matrix& n, Matrix& v) {
    auto arg = eval(atom->getStatement(0), n, v);
    
    // true если аргумент атомарный (не список)
    bool is_atom = (std::dynamic_pointer_cast<syntax_tree::ListNode>(arg) == nullptr);
    
    return std::make_shared<syntax_tree::LiteralBool>("LiteralBool", is_atom);
}

LiteralInt Emulator::evalAddNode(AddNode add, Matrix& n, Matrix& v) {
    auto left = eval(add->getStatement(0), n, v);
    auto right = eval(add->getStatement(1), n, v);

    if (auto left_lit = std::dynamic_pointer_cast<syntax_tree::LiteralInt>(left)) {
        if (auto right_lit = std::dynamic_pointer_cast<syntax_tree::LiteralInt>(right)) {
            return std::make_shared<syntax_tree::LiteralInt>("LiteralInt", left_lit->getValue() + right_lit->getValue());
        }
    }
    throw std::runtime_error("Add operation requires integer operands");
}

LiteralInt Emulator::evalSubNode(SubNode sub, Matrix& n, Matrix& v) {
    auto left = eval(sub->getStatement(0), n, v);
    auto right = eval(sub->getStatement(1), n, v);

    if (auto left_lit = std::dynamic_pointer_cast<syntax_tree::LiteralInt>(left)) {
        if (auto right_lit = std::dynamic_pointer_cast<syntax_tree::LiteralInt>(right)) {
            return std::make_shared<syntax_tree::LiteralInt>("LiteralInt", left_lit->getValue() - right_lit->getValue());
        }
    }
    throw std::runtime_error("Sub operation requires integer operands");
}

LiteralInt Emulator::evalMulNode(MulNode mul, Matrix& n, Matrix& v) {
    auto left = eval(mul->getStatement(0), n, v);
    auto right = eval(mul->getStatement(1), n, v);

    if (auto left_lit = std::dynamic_pointer_cast<syntax_tree::LiteralInt>(left)) {
        if (auto right_lit = std::dynamic_pointer_cast<syntax_tree::LiteralInt>(right)) {
            return std::make_shared<syntax_tree::LiteralInt>("LiteralInt", left_lit->getValue() * right_lit->getValue());
        }
    }
    throw std::runtime_error("Mul operation requires integer operands");
}

LiteralInt Emulator::evalDiveNode(DiveNode dive, Matrix& n, Matrix& v) {
    auto left = eval(dive->getStatement(0), n, v);
    auto right = eval(dive->getStatement(1), n, v);

    if (auto left_lit = std::dynamic_pointer_cast<syntax_tree::LiteralInt>(left)) {
        if (auto right_lit = std::dynamic_pointer_cast<syntax_tree::LiteralInt>(right)) {
            return std::make_shared<syntax_tree::LiteralInt>("LiteralInt", (int)left_lit->getValue() / right_lit->getValue());
        }
    }
    throw std::runtime_error("Dive operation requires integer operands");
}

LiteralInt Emulator::evalRemNode(RemNode rem, Matrix& n, Matrix& v) {
    auto left = eval(rem->getStatement(0), n, v);
    auto right = eval(rem->getStatement(1), n, v);

    if (auto left_lit = std::dynamic_pointer_cast<syntax_tree::LiteralInt>(left)) {
        if (auto right_lit = std::dynamic_pointer_cast<syntax_tree::LiteralInt>(right)) {
            return std::make_shared<syntax_tree::LiteralInt>("LiteralInt", left_lit->getValue() % right_lit->getValue());
        }
    }
    throw std::runtime_error("Rem operation requires integer operands");
}

LiteralBool Emulator::evalLeNode(LeNode le, Matrix& n, Matrix& v) {
    auto left = eval(le->getStatement(0), n, v);
    auto right = eval(le->getStatement(1), n, v);

    if (auto left_lit = std::dynamic_pointer_cast<syntax_tree::LiteralInt>(left)) {
        if (auto right_lit = std::dynamic_pointer_cast<syntax_tree::LiteralInt>(right)) {
            if (left_lit->getValue() <= right_lit->getValue()) {
                return std::make_shared<syntax_tree::LiteralBool>("LiteralBool", true);
            }
            else {
                return std::make_shared<syntax_tree::LiteralBool>("LiteralBool", false);
            }
        }
    }
    throw std::runtime_error("Le operation requires integer operands");
}

ListNode Emulator::evalConsNode(ConsNode cons, Matrix& n, Matrix& v) {
    auto left = eval(cons->getStatement(0), n, v);
    auto right = eval(cons->getStatement(1), n, v);

    auto new_cons = std::make_shared<syntax_tree::ListNode>("LIST");
    new_cons->addStatement(left);
    if (auto cons = std::dynamic_pointer_cast<syntax_tree::ListNode>(right)) {
        new_cons->addStatements(cons->getStatements());
        return new_cons;
    }
    else if (auto nil = std::dynamic_pointer_cast<syntax_tree::LiteralNil>(right)) {
        return new_cons;
    }
    
    throw std::runtime_error("Cons error!");
}

LiteralBool Emulator::evalEqualNode(EqualNode equal, Matrix& n, Matrix& v) {
    auto left = eval(equal->getStatement(0), n, v);
    auto right = eval(equal->getStatement(1), n, v);

    if (auto left_lit = std::dynamic_pointer_cast<syntax_tree::LiteralInt>(left)) {
        if (auto right_lit = std::dynamic_pointer_cast<syntax_tree::LiteralInt>(right)) {
            if (left_lit->getValue() == right_lit->getValue()) {
                return std::make_shared<syntax_tree::LiteralBool>("LiteralBool", true);
            }
            else {
                return std::make_shared<syntax_tree::LiteralBool>("LiteralBool", false);
            }
        }
    }
    throw std::runtime_error("Equal operation requires integer operands");
}

Node Emulator::evalCondNode(CondNode cond, Matrix& n, Matrix& v) {
    auto expr = eval(cond->getStatement(0), n, v);
    auto left = eval(cond->getStatement(1), n, v);
    auto right = eval(cond->getStatement(2), n, v);

    if (auto e = std::dynamic_pointer_cast<syntax_tree::LiteralBool>(expr)) {
        if (e->getValue()) {
            return left;
        }
        else {
            return right;
        }
    }
    
    throw std::runtime_error("Cond error!");
}