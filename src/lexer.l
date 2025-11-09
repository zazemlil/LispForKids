%{
#include "Parser.hpp"
#include "AST.h"

using namespace lisp_for_kids;
void set_input_file(yyscan_t scanner, const char* filename);

static int current_line = 1;
static int current_column = 1;

%}

%option reentrant batch noyywrap yylineno nodefault outfile="Scanner.cpp" header="Scanner.hpp"

%{
#define YY_USER_ACTION \
    do { \
        if (yylloc) { \
            yylloc->begin.line = yylloc->end.line = current_line; \
            yylloc->begin.column = current_column; \
            yylloc->end.column = current_column + yyleng - 1; \
        } \
        current_column += yyleng; \
    } while (0);
%}

%%

;.*                          { /* ignore comment */ }
[qQ][uU][oO][tT][eE]         { yylval->emplace<std::string>("QUOTE"); return Parser::token::T_QUOTE; }
[cC][aA][rR]                 { yylval->emplace<std::string>("CAR"); return Parser::token::T_CAR; }
[cC][dD][rR]                 { yylval->emplace<std::string>("CDR"); return Parser::token::T_CDR; }
[cC][oO][nN][sS]             { yylval->emplace<std::string>("CONS"); return Parser::token::T_CONS; }
[aA][tT][oO][mM]             { yylval->emplace<std::string>("ATOM"); return Parser::token::T_ATOM; }
[lL][iI][tT][eE][rR][aA][lL] { yylval->emplace<std::string>("LITERAL"); return Parser::token::T_LITERAL; }
[eE][qQ][uU][aA][lL]         { yylval->emplace<std::string>("EQUAL"); return Parser::token::T_EQUAL; }
[aA][dD][dD]                 { yylval->emplace<std::string>("ADD"); return Parser::token::T_ADD; }
[sS][uU][bB]                 { yylval->emplace<std::string>("SUB"); return Parser::token::T_SUB; }
[mM][uU][lL]                 { yylval->emplace<std::string>("MUL"); return Parser::token::T_MUL; }
[dD][iI][vV][eE]             { yylval->emplace<std::string>("DIVE"); return Parser::token::T_DIVE; }
[rR][eE][mM]                 { yylval->emplace<std::string>("REM"); return Parser::token::T_REM; }
[lL][eE]                     { yylval->emplace<std::string>("LE"); return Parser::token::T_LE; }
[cC][oO][nN][dD]             { yylval->emplace<std::string>("COND"); return Parser::token::T_COND; }
[lL][aA][mM][bB][dD][aA]     { yylval->emplace<std::string>("LAMBDA"); return Parser::token::T_LAMBDA; }
[lL][eE][tT]                 { yylval->emplace<std::string>("LET"); return Parser::token::T_LET; }
[lL][eE][tT][rR][eE][cC]     { yylval->emplace<std::string>("LETREC"); return Parser::token::T_LETREC; }
[nN][iI][lL]                 { yylval->emplace<std::string>("NIL"); return Parser::token::T_LITERAL_NIL; }
[tT][rR][uU][eE]             { yylval->emplace<std::string>("TRUE"); return Parser::token::T_LITERAL_TRUE; }
[fF][aA][lL][sS][eE]         { yylval->emplace<std::string>("FALSE"); return Parser::token::T_LITERAL_FALSE; }

"(" { return Parser::token::T_PARENTHESIS_OPEN; }
")" { return Parser::token::T_PARENTHESIS_CLOSE; }

[a-zA-z][a-zA-z|0-9]* { yylval->emplace<std::string>(std::string(yytext, yyleng)); return Parser::token::T_IDENTIFIER; }
[-]?[0-9]+ { yylval->emplace<int>(atoi(yytext)); return Parser::token::T_LITERAL_INT; }

\n { current_line++; current_column = 1; }
\r { current_column = 1; }
[ \t]+ {}

<<EOF>> { return Parser::token::T_END_OF_FILE; }
. { return yytext[0]; }

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
    if (parser.parse() == 0) {
        std::cout << "Parse success.\n";
    }
    else {
        std::cerr << "Parse error!\n";
    }
    
    yylex_destroy(scanner);

    return result;
}