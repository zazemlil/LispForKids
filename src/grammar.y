%{
#include <iostream>
#include <memory>
#include <string>
#include <cmath>
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

%code requires {
    #include "AST.h"
}

%code provides
{
    #define YY_DECL \
        int yylex(lisp_for_kids::Parser::semantic_type *yylval, yyscan_t yyscanner)
    YY_DECL;
}

%token <std::string> T_IDENTIFIER
%token <int> T_LITERAL_INT

%token T_PARENTHESIS_OPEN T_PARENTHESIS_CLOSE

%token <std::string> T_QUOTE T_CAR T_CDR T_CONS T_ATOM T_EQUAL T_ADD 
%token <std::string> T_SUB T_MUL T_DIVE T_REM T_LE T_COND T_LAMBDA T_LET T_LETREC 

%token T_END_OF_FILE

%type <std::unique_ptr<syntax_tree::ASTNode>> expr

%%

program: expr T_CAR T_IDENTIFIER T_LITERAL_INT T_END_OF_FILE {
    //($1)->print(0);
    result = syntax_tree::AST(std::move($1));
    std::cout << $2 << "\n";
    std::cout << $3 << "\n";
    std::cout << $4 << "\n";
    std::cout << "Success!" << std::endl;
    YYACCEPT;
};

expr: %empty {
    $$ = std::make_unique<syntax_tree::ASTNode>("testNode");
};

%%

void lisp_for_kids::Parser::error(const std::string& msg) {
    std::cerr << msg << '\n';
}