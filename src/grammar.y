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

%type <std::unique_ptr<syntax_tree::ASTNode>> s expr atom list application op1 op2 params bindings bindingsTail bind

%%

s: expr T_END_OF_FILE {
    std::unique_ptr<syntax_tree::ASTNode> test = std::make_unique<syntax_tree::ASTNode>("testNode"); 
    result = syntax_tree::AST(std::move(test));
    YYACCEPT;
};

expr: atom {}
    | T_PARENTHESIS_OPEN list T_PARENTHESIS_CLOSE {}
    | T_PARENTHESIS_OPEN application T_PARENTHESIS_CLOSE {};

atom: T_IDENTIFIER {}
    | T_LITERAL_INT {};

list: expr list {}
    | %empty {};
    
application: op1 expr {}
    | op2 expr expr {}
    | T_COND expr expr expr {}
    | T_LAMBDA T_PARENTHESIS_OPEN params T_PARENTHESIS_CLOSE expr {}
    | T_LET expr bindings {}
    | T_LETREC expr bindings {};

op1: T_QUOTE {}
    | T_CAR {}
    | T_CDR {}
    | T_ATOM {};

op2: T_ADD {}
    | T_SUB {}
    | T_MUL {}
    | T_DIVE {}
    | T_REM {}
    | T_LE {}
    | T_CONS {}
    | T_EQUAL {};

params: T_IDENTIFIER params {}
    | %empty {};

bindings: bind bindingsTail {};
bindingsTail: bind bindingsTail {}
    | %empty {};

bind: T_PARENTHESIS_OPEN T_IDENTIFIER expr T_PARENTHESIS_CLOSE {};

%%

void lisp_for_kids::Parser::error(const location_type& loc, const std::string& msg) {
    const char* text = yyget_text(scanner);
    int length = yyget_leng(scanner);
    
    std::cerr << msg << " at (Line: " << loc.begin.line << ", Column: " << loc.begin.column
            << ", Last token: '" << std::string(text, length) << "')\n";
}