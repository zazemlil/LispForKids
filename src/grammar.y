%{
#include "Scanner.hpp"
%}

%require "3.7.4"
%language "C++"
%defines "Parser.hpp"
%output "Parser.cpp"

%define api.parser.class {Parser}
%define api.namespace {lisp_for_kids}
%define api.value.type variant
%param {yyscan_t scanner}
%parse-param {syntax_tree::AST& result}

%locations

%code requires {
    #include "AST.h"
}

%code provides
{
    #define YY_DECL \
        int yylex(lisp_for_kids::Parser::semantic_type *yylval, \
                lisp_for_kids::Parser::location_type* yylloc, \
                yyscan_t yyscanner)
    YY_DECL;
}

%token <std::string> T_IDENTIFIER
%token <int> T_LITERAL_INT

%token T_PARENTHESIS_OPEN T_PARENTHESIS_CLOSE

%token <std::string> T_QUOTE T_CAR T_CDR T_CONS T_ATOM T_EQUAL T_ADD 
%token <std::string> T_SUB T_MUL T_DIVE T_REM T_LE T_COND T_LAMBDA T_LET T_LETREC 

%token T_END_OF_FILE

%type <std::shared_ptr<syntax_tree::ASTNode>> s expr atom list application unaryop binaryop ternaryop bind num id
%type <std::vector<std::shared_ptr<syntax_tree::ASTNode>>> params bindings bindingsTail

%%

s: expr T_END_OF_FILE {
    result = syntax_tree::AST($1);
    YYACCEPT;
};


expr: atom { $$ = $1; }
    | T_PARENTHESIS_OPEN list T_PARENTHESIS_CLOSE { $$ = $2; }
    | T_PARENTHESIS_OPEN application T_PARENTHESIS_CLOSE { $$ = $2; };


atom: id { $$ = $1; }
    | num { $$ = $1; };


list: expr list {
        auto l = std::make_shared<syntax_tree::ASTNode>("CONS");
        l->addStatement($1);
        l->addStatement($2);
        $$ = l;
    }
    | %empty { $$ = std::make_shared<syntax_tree::ASTNode>("NIL"); };
    

application: unaryop expr { $1->addStatement($2); $$ = $1; }
    | binaryop expr expr { $1->addStatement($2); $1->addStatement($3); $$ = $1; }
    | ternaryop expr expr expr { $1->addStatement($2); $1->addStatement($3); $1->addStatement($4); $$ = $1; }
    | T_LAMBDA T_PARENTHESIS_OPEN params T_PARENTHESIS_CLOSE expr {
        auto l = std::make_shared<syntax_tree::ASTNode>("LAMBDA");
        l->setStatements($3); 
        l->addStatement($5);
        $$ = l;
    }
    | T_LET expr bindings {
        auto l = std::make_shared<syntax_tree::ASTNode>("LET");
        l->addStatement($2);
        l->addStatements($3); 
        $$ = l;
    }
    | T_LETREC expr bindings {
        auto l = std::make_shared<syntax_tree::ASTNode>("LETREC");
        l->addStatement($2);
        l->addStatements($3); 
        $$ = l;
    };


unaryop: T_QUOTE { $$ = std::make_shared<syntax_tree::ASTNode>("QUOTE"); }
    | T_CAR { $$ = std::make_shared<syntax_tree::ASTNode>("CAR"); }
    | T_CDR { $$ = std::make_shared<syntax_tree::ASTNode>("CDR"); }
    | T_ATOM { $$ = std::make_shared<syntax_tree::ASTNode>("ATOM"); };


binaryop: T_ADD { $$ = std::make_shared<syntax_tree::ASTNode>("ADD"); }
    | T_SUB { $$ = std::make_shared<syntax_tree::ASTNode>("SUB"); }
    | T_MUL { $$ = std::make_shared<syntax_tree::ASTNode>("MUL"); }
    | T_DIVE { $$ = std::make_shared<syntax_tree::ASTNode>("DIVE"); }
    | T_REM { $$ = std::make_shared<syntax_tree::ASTNode>("REM"); }
    | T_LE { $$ = std::make_shared<syntax_tree::ASTNode>("LE"); }
    | T_CONS { $$ = std::make_shared<syntax_tree::ASTNode>("CONS"); }
    | T_EQUAL { $$ = std::make_shared<syntax_tree::ASTNode>("EQUAL"); };

ternaryop: T_COND { $$ = std::make_shared<syntax_tree::ASTNode>("COND"); };

params: id params {
        $2.insert($2.begin(), $1);
        $$ = $2;
    }
    | %empty { $$ = std::vector<std::shared_ptr<syntax_tree::ASTNode>>(); };


bindings: bind bindingsTail {
    $2.insert($2.begin(), $1);
    $$ = $2;
};
bindingsTail: bind bindingsTail {
        $2.insert($2.begin(), $1);
        $$ = $2;
    }
    | %empty { $$ = std::vector<std::shared_ptr<syntax_tree::ASTNode>>(); };


bind: T_PARENTHESIS_OPEN id expr T_PARENTHESIS_CLOSE { 
    auto b = std::make_shared<syntax_tree::ASTNode>("ASSIGN");
    b->addStatement($2); b->addStatement($3);
    $$ = b;
};

num: T_LITERAL_INT { $$ = std::make_shared<syntax_tree::LiteralInt>("LiteralInt", $1); };
id: T_IDENTIFIER { $$ = std::make_shared<syntax_tree::Identifier>("Identifier", $1); };

%%

void lisp_for_kids::Parser::error(const location_type& loc, const std::string& msg) {
    const char* text = yyget_text(scanner);
    int length = yyget_leng(scanner);
    
    std::cerr << msg << " at (Line: " << loc.begin.line << ", Column: " << loc.begin.column
            << ", Last token: '" << std::string(text, length) << "')\n";
}