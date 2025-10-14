%{
#include <cstdlib>
#include "Parser.hpp"
#include "AST.h"

using namespace lisp_for_kids;
void set_input_file(yyscan_t scanner, const char* filename);

%}

%option reentrant batch noyywrap noyylineno nodefault outfile="Scanner.cpp" header="Scanner.hpp"

%%

"QUOTE"|"quote"    { return Parser::token::T_QUOTE; }
"CAR"|"car"        { return Parser::token::T_CAR; }
"CDR"|"cdr"        { return Parser::token::T_CDR; }
"CONS"|"cons"      { return Parser::token::T_CONS; }
"ATOM"|"atom"      { return Parser::token::T_ATOM; }
"EQUAL"|"equal"    { return Parser::token::T_EQUAL; }
"ADD"|"add"        { return Parser::token::T_ADD; }
"SUB"|"sub"        { return Parser::token::T_SUB; }
"MUL"|"mul"        { return Parser::token::T_MUL; }
"DIV"|"div"        { return Parser::token::T_DIV; }
"REM"|"rem"        { return Parser::token::T_REM; }
"LEQ"|"leq"        { return Parser::token::T_LEQ; }
"COND"|"cond"      { return Parser::token::T_COND; }
"LAMBDA"|"lambda"  { return Parser::token::T_LAMBDA; }
"LET"|"let"        { return Parser::token::T_LET; }
"LETREC"|"letrec"  { return Parser::token::T_LETREC; }

"(" { return Parser::token::T_PARENTHESIS_OPEN; }
")" { return Parser::token::T_PARENTHESIS_CLOSE; }

[a-zA-z][a-zA-z|0-9]* { yylval->emplace<std::string>(std::string(yytext, yyleng)); return Parser::token::T_IDENTIFIER; }
[+-]?[0-9]+ { yylval->emplace<int>(atoi(yytext)); return Parser::token::T_LITERAL_INT; }

[ \t\n\r ]

<<EOF>> { return Parser::token::T_END_OF_FILE; }
. {}

%%

void set_input_file(yyscan_t scanner, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file) {
        yyset_in(file, scanner);
    } else {
        std::cerr << "Cannot open file: " << filename << std::endl;
        exit(1);
    }
}

syntax_tree::AST analize(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return syntax_tree::AST();
    }
    
    yyscan_t scanner;
    yylex_init(&scanner);
    
    set_input_file(scanner, argv[1]);
    
    syntax_tree::AST result;
    lisp_for_kids::Parser parser{ scanner, result };
    if (parser.parse() != 0) {
        std::cerr << "Parse error!" << std::endl;
    }
    
    yylex_destroy(scanner);

    return result;
}