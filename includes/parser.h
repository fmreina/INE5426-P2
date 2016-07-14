/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_INCLUDES_PARSER_H_INCLUDED
# define YY_YY_INCLUDES_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    T_NEW_LINE = 258,
    T_PLUS = 259,
    T_MINUS = 260,
    T_TIMES = 261,
    T_DIVIDE = 262,
    T_GREATER = 263,
    T_GREATER_EQUALS = 264,
    T_SMALLER = 265,
    T_SMALLER_EQUALS = 266,
    T_EQUALS = 267,
    T_DIFFERENT = 268,
    T_NOT = 269,
    T_AND = 270,
    T_OR = 271,
    T_INT = 272,
    T_REAL = 273,
    T_BOOL = 274,
    T_WORD = 275,
    T_DEFINITION = 276,
    T_TYPE_VOID = 277,
    T_TYPE_INT = 278,
    T_TYPE_REAL = 279,
    T_TYPE_BOOL = 280,
    T_WHILE = 281,
    T_IF = 282,
    T_THEN = 283,
    T_ELSE = 284,
    T_FROM = 285,
    T_UNTIL = 286,
    T_DEF_FUNCTION = 287,
    T_DECL_FUNCTION = 288,
    T_RETURN = 289,
    T_ASSIGN = 290,
    T_COMMA = 291,
    T_COLON = 292,
    T_SEMICOLON = 293,
    T_OPEN_PARENTHESIS = 294,
    T_CLOSE_PARENTHESIS = 295,
    T_OPEN_BRACKETS = 296,
    T_CLOSE_BRACKETS = 297,
    T_OPEN_BRACES = 298,
    T_CLOSE_BRACES = 299,
    U_NEGATIVE = 300
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 22 "./syntax/parser.y" /* yacc.c:1909  */

	const char* string;
	
	AST::Node *node;
	AST::Block *block;
	AST::VariableDeclaration *var;
	AST::ArrayDeclaration *arr;
	AST::WhileBlock *whileBlock;
	AST::IfBlock *ifBlock;
	AST::FromTil_Block *fromtilBlock;
	AST::FunctionDeclaration *fun;
	AST::FunctionDefinition *func_def;
	// AST::FunctionReturn *func_return;
	AST::FunctionBody *body;
	void* novalue;

#line 117 "./includes/parser.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_INCLUDES_PARSER_H_INCLUDED  */
