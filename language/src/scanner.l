/*
 *	Scanner
 *	references: 
 *	 - llpilla/compiler_examples/allen (github)
 *	 - Aho, Alfred, et al - Compilers: principles, techniques, and tools - 2nd ed. 2007 Pearson Education
 *	 - Levine, John - Flex & Bison - Unix text processing tools - 2009 O'Reilly Media
 *		*page 21 for the regular exprecion on <real>
 */
%{
#include <string>
#include "ast.h"
#include "parser.h"

extern int yylineno;

void yyerror(const char* s, ...);

%}

%option noyywrap nodefault yylineno

digit[0-9]+
real [0-9]*\.?[0-9]+|[0-9]+\.[0-9]*
word[a-zA-Z]+

%%

[ \t]	; // to ignore white spaces
\n		{ return T_NEW_LINE; }

","		{ return T_COMMA; }
":"		{ return T_COLON; }
";"		{ return T_SEMICOLON; } // semicolon indicates the end of the current line.

"="	{ return T_ASSIGN; }

"int"	{ return T_TYPE_INT; }
"real"	{ return T_TYPE_REAL; }
"bool"	{ return T_TYPE_BOOL; }

"true"	{ yylval.string = strdup(yytext); return T_BOOL; } // received as string as the objective is only printing
"false"	{ yylval.string = strdup(yytext); return T_BOOL; } // received as string as the objective is only printing
{digit}	{ yylval.string = strdup(yytext); return T_INT; } // received as string as the objective is only printing
{real}	{ yylval.string = strdup(yytext); return T_REAL; } // received as string as the objective is only printing
{word}	{ yylval.string = strdup(yytext); return T_WORD; } // received as string as the objective is only printing

.		{ yyerror("Token desconhecido! %c\n", *yytext); } // inform of any unknow character (PT)

%%
void yyerror(const char *s, ...){
	va_list ap;
	va_start(ap, s);
	std::fprintf(stderr, "Line %d: error: ", yylineno);
	std::vfprintf(stderr,s,ap);
}
