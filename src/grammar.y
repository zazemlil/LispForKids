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

%type <std::unique_ptr<syntax_tree::ASTNode>> s expr atom list application unaryop binaryop ternaryop bind num id
%type <std::vector<std::unique_ptr<syntax_tree::ASTNode>>> params bindings bindingsTail

%%

s: expr T_END_OF_FILE {
    result = syntax_tree::AST(std::move($1));
    YYACCEPT;
};


expr: atom { $$ = std::move($1); }
    | T_PARENTHESIS_OPEN list T_PARENTHESIS_CLOSE { $$ = std::move($2); }
    | T_PARENTHESIS_OPEN application T_PARENTHESIS_CLOSE { $$ = std::move($2); };


atom: id { $$ = std::move($1); }
    | num { $$ = std::move($1); };


list: expr list {
        auto l = std::make_unique<syntax_tree::ASTNode>("CONS");
        l->addStatement(std::move($1));
        l->addStatement(std::move($2));
        $$ = std::move(l);
    }
    | %empty { $$ = std::make_unique<syntax_tree::ASTNode>("NIL"); };
    

application: unaryop expr { $1->addStatement(std::move($2)); $$ = std::move($1); }
    | binaryop expr expr { $1->addStatement(std::move($2)); $1->addStatement(std::move($3)); $$ = std::move($1); }
    | ternaryop expr expr expr { $1->addStatement(std::move($2)); $1->addStatement(std::move($3)); $1->addStatement(std::move($4)); $$ = std::move($1); }
    | T_LAMBDA T_PARENTHESIS_OPEN params T_PARENTHESIS_CLOSE expr {
        auto l = std::make_unique<syntax_tree::ASTNode>("LAMBDA");
        l->setStatements(std::move($3)); 
        l->addStatement(std::move($5));
        $$ = std::move(l);
    }
    | T_LET expr bindings {
        auto l = std::make_unique<syntax_tree::ASTNode>("LET");
        l->addStatement(std::move($2));
        l->addStatements(std::move($3)); 
        $$ = std::move(l);
    }
    | T_LETREC expr bindings {
        auto l = std::make_unique<syntax_tree::ASTNode>("LETREC");
        l->addStatement(std::move($2));
        l->addStatements(std::move($3)); 
        $$ = std::move(l);
    };


unaryop: T_QUOTE { $$ = std::make_unique<syntax_tree::ASTNode>("QUOTE"); }
    | T_CAR { $$ = std::make_unique<syntax_tree::ASTNode>("CAR"); }
    | T_CDR { $$ = std::make_unique<syntax_tree::ASTNode>("CDR"); }
    | T_ATOM { $$ = std::make_unique<syntax_tree::ASTNode>("ATOM"); };


binaryop: T_ADD { $$ = std::make_unique<syntax_tree::ASTNode>("ADD"); }
    | T_SUB { $$ = std::make_unique<syntax_tree::ASTNode>("SUB"); }
    | T_MUL { $$ = std::make_unique<syntax_tree::ASTNode>("MUL"); }
    | T_DIVE { $$ = std::make_unique<syntax_tree::ASTNode>("DIVE"); }
    | T_REM { $$ = std::make_unique<syntax_tree::ASTNode>("REM"); }
    | T_LE { $$ = std::make_unique<syntax_tree::ASTNode>("LE"); }
    | T_CONS { $$ = std::make_unique<syntax_tree::ASTNode>("CONS"); }
    | T_EQUAL { $$ = std::make_unique<syntax_tree::ASTNode>("EQUAL"); };

ternaryop: T_COND { $$ = std::make_unique<syntax_tree::ASTNode>("COND"); };

params: id params {
        $2.insert($2.begin(), std::move($1));
        $$ = std::move($2);
    }
    | %empty { $$ = std::vector<std::unique_ptr<syntax_tree::ASTNode>>(); };


bindings: bind bindingsTail {
    $2.insert($2.begin(), std::move($1));
    $$ = std::move($2);
};
bindingsTail: bind bindingsTail {
        $2.insert($2.begin(), std::move($1));
        $$ = std::move($2);
    }
    | %empty { $$ = std::vector<std::unique_ptr<syntax_tree::ASTNode>>(); };


bind: T_PARENTHESIS_OPEN id expr T_PARENTHESIS_CLOSE { 
    auto b = std::make_unique<syntax_tree::ASTNode>("ASSIGN");
    b->addStatement(std::move($2)); b->addStatement(std::move($3));
    $$ = std::move(b);
};

num: T_LITERAL_INT { $$ = std::make_unique<syntax_tree::LiteralInt>("LiteralInt", $1); };
id: T_IDENTIFIER { $$ = std::make_unique<syntax_tree::Identifier>("Identifier", $1); };

%%

void lisp_for_kids::Parser::error(const location_type& loc, const std::string& msg) {
    const char* text = yyget_text(scanner);
    int length = yyget_leng(scanner);
    
    std::cerr << msg << " at (Line: " << loc.begin.line << ", Column: " << loc.begin.column
            << ", Last token: '" << std::string(text, length) << "')\n";
}