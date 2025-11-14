#include "Emulator.h"
#include <iostream>

syntax_tree::AST Emulator::eval(syntax_tree::AST ast) {
    // Matrix n = {
    //     {std::make_shared<syntax_tree::Identifier>("Identifier", "a")}
    // };
    // Matrix v = {
    //     {std::make_shared<syntax_tree::LiteralInt>("LiteralInt", 1)}
    // };
    Matrix n, v;
    Node root = eval(ast.getRoot(), n, v);
    return syntax_tree::AST(root);
}

Node Emulator::eval(Node e, Matrix& n, Matrix& v) {
    if (auto litInt = std::dynamic_pointer_cast<syntax_tree::LiteralInt>(e)) {
        return evalLiteralInt(litInt, n, v);
    }
    else if (auto litBool = std::dynamic_pointer_cast<syntax_tree::LiteralBool>(e)) {
        return evalLiteralBool(litBool, n, v);
    }
    else if (auto litNil = std::dynamic_pointer_cast<syntax_tree::LiteralNil>(e)) {
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
    else if (auto literal = std::dynamic_pointer_cast<syntax_tree::LiteralNode>(e)) {
        return evalLiteralNode(literal, n, v);
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
    else if (auto lam = std::dynamic_pointer_cast<syntax_tree::LambdaNode>(e)) {
        return evalLambdaNode(lam, n, v);
    }
    else if (auto let = std::dynamic_pointer_cast<syntax_tree::LetNode>(e)) {
        return evalLetNode(let, n, v);
    }
    else if (auto letrec = std::dynamic_pointer_cast<syntax_tree::LetrecNode>(e)) {
        return evalLetrecNode(letrec, n, v);
    }
    else if (auto list = std::dynamic_pointer_cast<syntax_tree::ListNode>(e)) {
        return evalFuncCall(list, n, v);
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

Node Emulator::evalIdentifier(Identifier id, Matrix& n, Matrix& v) {
    return assoc(id, n, v);
}

Node Emulator::evalQuoteNode(QuoteNode quote, Matrix& n, Matrix& v) {
    return quote->getStatement(0);
}

Node Emulator::evalCarNode(CarNode car, Matrix& n, Matrix& v) {
    auto c = eval(car->getStatement(0), n, v);
    
    if (auto nil = std::dynamic_pointer_cast<syntax_tree::LiteralNil>(c)) {
        return nil;
    } 
    else if (c->getStatementCount() >= 1) {
        auto first = c->getStatement(0);
        if (auto list = std::dynamic_pointer_cast<syntax_tree::ListNode>(c)) {
            return first;
        }
    }
    
    throw std::runtime_error("Car error: arg must be Nil or List");
}

Node Emulator::evalCdrNode(CdrNode cdr, Matrix& n, Matrix& v) {
    auto c = eval(cdr->getStatement(0), n, v);
    
    if (auto nil = std::dynamic_pointer_cast<syntax_tree::LiteralNil>(c)) {
        return nil;
    } else if (auto list = std::dynamic_pointer_cast<syntax_tree::ListNode>(c)) {
        if (c->getStatementCount() >= 2) {
            auto new_list = std::make_shared<syntax_tree::ListNode>("LIST");
            for (size_t i = 1; i < c->getStatementCount(); ++i) {
                new_list->addStatement(c->getStatement(i));
            }
            return new_list;
        } else if (c->getStatementCount() == 1) {
            return std::make_shared<syntax_tree::LiteralNil>("NIL");
        }
    }
    
    throw std::runtime_error("Cdr error: arg must be Nil or List");
}

Node Emulator::evalAtomNode(AtomNode atom, Matrix& n, Matrix& v) {
    auto arg = eval(atom->getStatement(0), n, v);
    
    // true, если аргумент атомарный (не список)
    bool is_atom = (std::dynamic_pointer_cast<syntax_tree::ListNode>(arg) == nullptr);
    
    return std::make_shared<syntax_tree::LiteralBool>("LiteralBool", is_atom);
}

Node Emulator::evalLiteralNode(LiteralNode literal, Matrix& n, Matrix& v) {
    auto arg = eval(literal->getStatement(0), n, v);
    bool isLiteral = false;

    if (auto lit_int = std::dynamic_pointer_cast<syntax_tree::LiteralInt>(arg)) {
        isLiteral = true;
    }
    else if (auto lit_bool = std::dynamic_pointer_cast<syntax_tree::LiteralBool>(arg)) {
        isLiteral = true;
    }
    else if (auto lit_nil = std::dynamic_pointer_cast<syntax_tree::LiteralNil>(arg)) {
        isLiteral = true;
    }
    return std::make_shared<syntax_tree::LiteralBool>("LiteralBool", isLiteral);
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
            return std::make_shared<syntax_tree::LiteralInt>("LiteralInt", left_lit->getValue() / right_lit->getValue());
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
    
    throw std::runtime_error("Cons error: second param must be List or Nil");
}

LiteralBool Emulator::evalEqualNode(EqualNode equal, Matrix& n, Matrix& v) {
    auto left = eval(equal->getStatement(0), n, v);
    auto right = eval(equal->getStatement(1), n, v);

    bool left_is_atom = (std::dynamic_pointer_cast<syntax_tree::ListNode>(left) == nullptr);
    bool right_is_atom = (std::dynamic_pointer_cast<syntax_tree::ListNode>(right) == nullptr);

    if (left_is_atom || right_is_atom) {
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
        if (auto left_lit = std::dynamic_pointer_cast<syntax_tree::LiteralBool>(left)) {
            if (auto right_lit = std::dynamic_pointer_cast<syntax_tree::LiteralBool>(right)) {
                if (left_lit->getValue() == right_lit->getValue()) {
                    return std::make_shared<syntax_tree::LiteralBool>("LiteralBool", true);
                }
                else {
                    return std::make_shared<syntax_tree::LiteralBool>("LiteralBool", false);
                }
            }
        }
        if (auto left_lit = std::dynamic_pointer_cast<syntax_tree::Identifier>(left)) {
            if (auto right_lit = std::dynamic_pointer_cast<syntax_tree::Identifier>(right)) {
                if (left_lit->getValue() == right_lit->getValue()) {
                    return std::make_shared<syntax_tree::LiteralBool>("LiteralBool", true);
                }
                else {
                    return std::make_shared<syntax_tree::LiteralBool>("LiteralBool", false);
                }
            }
        }
        if (left->getNodeType() == right->getNodeType()) {
            return std::make_shared<syntax_tree::LiteralBool>("LiteralBool", true);
        }
        return std::make_shared<syntax_tree::LiteralBool>("LiteralBool", false);
    }

    throw std::runtime_error("Equal operation requires 1 or 2 atom operands");
}

Node Emulator::evalCondNode(CondNode cond, Matrix& n, Matrix& v) {
    auto expr = eval(cond->getStatement(0), n, v); 

    if (auto e = std::dynamic_pointer_cast<syntax_tree::LiteralBool>(expr)) {
        if (e->getValue()) {
            return eval(cond->getStatement(1), n, v);;
        }
        else {
            return eval(cond->getStatement(2), n, v);
        }
    }
    
    throw std::runtime_error("Cond error!");
}

Node Emulator::matrixToListNode(Matrix& matrix) {
    auto matrix_list = std::make_shared<syntax_tree::ListNode>("LIST");
    for (auto& row : matrix) {
        for (auto& element : row) {
            matrix_list->addStatement(element);
        }
    }
    return matrix_list;
}

FuncClosureNode Emulator::evalLambdaNode(LambdaNode lambda, Matrix& n, Matrix& v) {
    int size = lambda->getStatementCount();
    auto params = std::make_shared<syntax_tree::ListNode>("LIST");
    for (int i = 0; i < size-1; i++) {
        params->addStatement(lambda->getStatement(i));  
    }  
    auto body = lambda->getStatement(size-1);
    
    // (n v) = cons(n, cons(v, nil))
    auto context_list = std::make_shared<syntax_tree::ListNode>("LIST"); // CONTEXT

    context_list->addStatement(matrixToListNode(n)); 
    context_list->addStatement(matrixToListNode(v));
    
    // (y e) = cons(y, cons(e, nil))
    auto function_part = std::make_shared<syntax_tree::ListNode>("LIST"); // FUNCTION_PART
    function_part->addStatement(params); 
    function_part->addStatement(body);
    
    // zam = cons((y e), cons((n v), nil))
    auto closure = std::make_shared<syntax_tree::FuncClosureNode>("CLOSURE");
    closure->addStatement(function_part);   // (y e)
    closure->addStatement(context_list);    // (n v)
    
    return closure;
}

Node Emulator::assoc(Identifier id, Matrix& n, Matrix& v) {
    auto id_value = id->getValue();
    
    if (n.size() != v.size()) {
        throw std::runtime_error("Assoc: names and values matrices have different sizes");
    }
    
    for (size_t i = 0; i < n.size(); ++i) {
        auto& names_row = n[i];
        auto& values_row = v[i];
        
        if (names_row.size() != values_row.size()) {
            throw std::runtime_error("Assoc: names and values row sizes mismatch");
        }
        
        for (size_t j = 0; j < names_row.size(); ++j) {
            if (auto identifier = std::dynamic_pointer_cast<syntax_tree::Identifier>(names_row[j])) {
                if (identifier->getValue() == id_value) {
                    return values_row[j];
                }
            }
        }
    }

    throw std::runtime_error("Assoc: variable '" + id_value + "' not found");
}

Node Emulator::evalFuncCall(ListNode list, Matrix& n, Matrix& v) {
    // (x1 ... xk)
    std::vector<std::shared_ptr<syntax_tree::ASTNode>> evaluated_args;
    for (int i = 1; i < list->getStatementCount(); i++) {
        auto statement = list->getStatement(i);
        auto evaluated_arg = eval(statement, n, v);
        evaluated_args.push_back(evaluated_arg);
    }

    // e0
    auto func_closure_node = eval(list->getStatement(0), n, v);

    if (auto closure = std::dynamic_pointer_cast<syntax_tree::FuncClosureNode>(func_closure_node)) {
        if (closure->getStatement(0)->getStatement(0)->getStatementCount() != list->getStatementCount()-1) {
            throw std::runtime_error("Function call: params count error");
        }
        auto closure_arg_names = closure->getStatement(0)->getStatement(0)->getStatements();

        Matrix new_n = {};
        Matrix new_v = {};
        new_n.insert(new_n.begin(), closure_arg_names);
        new_v.insert(new_v.begin(), evaluated_args); 

        new_n.insert(new_n.begin(), closure->getStatement(1)->getStatement(0)->getStatements());
        new_v.insert(new_v.begin(), closure->getStatement(1)->getStatement(1)->getStatements());
        
        return eval(closure->getStatement(0)->getStatement(1), new_n, new_v);
    } else {
        func_closure_node->printRec(0, 5);
        throw std::runtime_error("Function call: first element must be a closure");
    }
}

Node Emulator::evalLetNode(LetNode let, Matrix& n, Matrix& v) {
    auto expr = let->getStatement(0);

    // (e1 ... ek)
    std::vector<std::shared_ptr<syntax_tree::ASTNode>> variables_values;
    std::vector<std::shared_ptr<syntax_tree::ASTNode>> variables_names;
    for (int i = 1; i < let->getStatementCount(); i++) {
        auto statement = let->getStatement(i);
        auto evaluated_arg = eval(statement->getStatement(1), n, v);
        variables_names.push_back(statement->getStatement(0));
        variables_values.push_back(evaluated_arg);
    }

    // nv refresh
    n.insert(n.begin(), variables_names);
    v.insert(v.begin(), variables_values);

    return eval(expr, n, v);
}

Node Emulator::evalLetrecNode(LetrecNode letrec, Matrix& n, Matrix& v) {
    auto expr = letrec->getStatement(0);

    std::vector<std::shared_ptr<syntax_tree::ASTNode>> variables_names; 
    std::vector<std::shared_ptr<syntax_tree::ASTNode>> variables_values;
    for (int i = 1; i < letrec->getStatementCount(); i++) {
        auto statement = letrec->getStatement(i);
        variables_names.push_back(statement->getStatement(0));
        auto evaluated_arg = std::make_shared<syntax_tree::FuncClosureNode>("OMEGA");
        variables_values.push_back(evaluated_arg);
    }
    n.insert(n.begin(), variables_names); // n`
    v.insert(v.begin(), variables_values); //v`

    //printMatrix(n, v);

    //z
    // (letrec
    //     (sum (quote(1 2 3 4 -5))) (
    //         sum (
    //             lambda (a) (
    //                     cond 
    //                         (equal a (quote())) 
    //                         (quote 0) 
    //                         (add (car a) (sum (cdr a))) 
    //             )
    //         )
    //     )
    // )
    // для этого примера список z будет состоять из замыкания с контекстом ((sum) (OMEGA))
    // а если бы в окружении была переменная `a` с значением 123, то замыкание = ((sum a) (OMEGA 123))

    std::vector<std::shared_ptr<syntax_tree::ASTNode>> z; 
    for (int i = 1; i < letrec->getStatementCount(); i++) {
        auto statement = letrec->getStatement(i);
        auto evaluated_arg = eval(statement->getStatement(1), n, v);
        z.push_back(evaluated_arg);
    }
    
    complete(v, z);

    return eval(expr, n, v);
}

Matrix& Emulator::complete(Matrix& v, std::vector<std::shared_ptr<syntax_tree::ASTNode>>& z) {
    for (size_t i = 0; i < z.size(); i++) {
        if (typeid(*v[0][i]) != typeid(*z[i])) {
            throw std::runtime_error("Letrec: local definitions can only be closures.");
        }
        *v[0][i] = *z[i];
    }
    return v;
}

Node Emulator::evalClosure(FuncClosureNode closure, Matrix& n, Matrix& v) {
    Matrix local_n = {closure->getStatement(1)->getStatement(0)->getStatements()};
    Matrix local_v = {closure->getStatement(1)->getStatement(1)->getStatements()};

    return eval(closure->getStatement(0)->getStatement(1), n, v);
}

void Emulator::printMatrixFlat(Matrix& n, Matrix& v) {
    std::cout << "[";
    for (size_t i = 0; i < n.size(); ++i) {
        auto& names_row = n[i];
        auto& values_row = v[i];
        std::cout << "\n\t{";
        for (size_t j = 0; j < names_row.size(); ++j) {
            std::cout << "\n\t\t";
            names_row[j]->printFlat();
            std::cout << " = ";
            values_row[j]->printFlat();
        }
        std::cout << "\n\t}";
    }
    std::cout << "\n]\n";
}

void Emulator::printMatrix(Matrix& n, Matrix& v) {
    std::cout << "\n\n((------------------------\nn=[";
    for (size_t i = 0; i < n.size(); ++i) {
        auto& names_row = n[i];
        std::cout << "\n\t{";
        for (size_t j = 0; j < names_row.size(); ++j) {
            std::cout << "\n";
            names_row[j]->print(5);
        }
        std::cout << "\n\t}";
    }
    std::cout << "\n] \nv=[";
    for (size_t i = 0; i < v.size(); ++i) {
        auto& values_row = v[i];
        std::cout << "\n\t{";
        for (size_t j = 0; j < values_row.size(); ++j) {
            std::cout << "\n";
            values_row[j]->print(5);
        }
        std::cout << "\n\t}";
    }
    std::cout << "\n))------------------------\n\n";
}