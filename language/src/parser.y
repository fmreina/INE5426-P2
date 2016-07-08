// Parser - references as shown on README.md
%{
	#include "ast.h"
	#include "symbolTable.h"
	using namespace std;
	using namespace AST;
	
	AST::Block* programRoot; /* root node of program AST */
	ST::SymbolTable symTab; /* global symbol table -  this is visible in the global scope */
	ST::SymbolTable symTabLocal;

	extern int yylex();
	extern void yyerror(const char* s, ...);
	
%}

/* 
 *	yylval == %union 
 * 	union informs the values data can be stored 
 */
%union {
	const char* string;
	
	AST::Node *node;
	AST::Block *block;
	AST::VariableDeclaration *var;
	AST::ArrayDeclaration *arr;
	AST::WhileBlock *whileBlock;
	AST::IfBlock *ifBlock;
	AST::FromTil_Block *fromtilBlock;
	// AST::FunctionDeclaration *fun;
	// AST::FunctionDefinition *func_def;
	// AST::FunctionReturn *func_return;
	// AST::FunctionBody *body;
	// AST::TypeDef *type_def;
	// AST::TypeBody *type_body;
}

/*
 *	token defines terminal Symbols (tokens)
 */
%token T_NEW_LINE

%token T_PLUS
%token T_MINUS
%token T_TIMES
%token T_DIVIDE
%token T_GREATER
%token T_GREATER_EQUALS	
%token T_SMALLER
%token T_SMALLER_EQUALS	
%token T_EQUALS	
%token T_DIFFERENT	
%token T_NOT	
%token T_AND	
%token T_OR	

%token <string> T_INT
%token <string> T_REAL
%token <string> T_BOOL
%token <string> T_WORD

%token T_DEFINITION
%token T_TYPE_INT
%token T_TYPE_REAL
%token T_TYPE_BOOL
%token T_WHILE
%token T_IF
%token T_THEN
%token T_ELSE
%token T_FROM
%token T_UNTIL
// %token T_DECL_FUNCTION
// %token T_DEF_FUNCTION
// %token T_END_DEF
// %token T_RETURN
// %token T_END_IF
// %token T_DO
// %token T_END_WHILE
// %token T_DEF_TYPE

%token T_ASSIGN
%token T_COMMA
%token T_COLON
%token T_SEMICOLON
%token T_OPEN_PARENTHESIS
%token T_CLOSE_PARENTHESIS
%token T_OPEN_BRACKETS
%token T_CLOSE_BRACKETS
%token T_OPEN_BRACES
%token T_CLOSE_BRACES

/* 
 *	type defines the type of our nonterminal symbols.
 *	Types should match the names used in the union.
 *	Example: %type<node> expr
 */
%type <block> program
%type <block> block
%type <node> line
%type <NULL> new_line
%type <node> declaration
%type <node> type
%type <var> variable_list
%type <node> assignment
%type <node> expression 
%type <node> target
%type <string> size
%type <arr> array_list
%type <node> target_array
%type <node> scope
%type <whileBlock> while_scope
%type <ifBlock> if_scope
%type <fromtilBlock> from_scope
// %type <fun> function_list
// %type <func_def> def_func
// %type <node> parameters
// %type <node> param
// %type <var> variable_param
// %type <arr> array_param
// %type <node> func_signature
// %type <body> func_body
// %type <func_return> return
// %type <type_body> type_body
// %type <type_def> def_type
 
/*
 *	Operator precedence for mathematical operators
 *	The latest it is listed, the highest the precedence
 */
%left T_DEFINITION
%left T_PLUS T_MINUS
%left T_TIMES T_DIVIDE
%left T_AND T_OR
%left T_NOT
%left T_GREATER T_GREATER_EQUALS T_SMALLER T_SMALLER_EQUALS T_EQUALS T_DIFFERENT
%left T_CLOSE_PARENTHESIS
%left T_OPEN_PARENTHESIS
%right U_NEGATIVE
%nonassoc error
 
/*
 *	starting rule 
 */
%start program
 
%%
/*
 *	Grammar structure based on the example presented by llpilla @github
 */

/*
 *	A program is made of many lines (blocks) 
 */
program : block { programRoot = $1; }
 		;
 	
/*
 *	Each group of lines can be a single line or 
 *	a group of lines followed by a single line
 */
block :	line { $$ = new AST::Block(); if ($1 != NULL) $$->lines.push_back($1); }
 		| block line { if($2 != NULL) $1->lines.push_back($2); }
 		;

/*
 *	A line may be a declaration or an assignment
 */ 		
line :	declaration T_SEMICOLON new_line { $$ = $1; 
												// for(auto it = symTab.entryList.cbegin(); it != symTab.entryList.cend(); ++it){
												// 	std::cout << "Gram:line:symbol: " << it->first << " " << TYPE::maleName[it->second.type]<<endl;
												// }
											}
		| assignment T_SEMICOLON new_line
		| line new_line 
		| scope new_line
		|expression T_SEMICOLON new_line // only for testing
		// | def_func new_line { $$ = $1; }
		// | def_type new_line { $$ = $1; }
		// | declaration T_ASSIGN expression T_SEMICOLON new_line { new AST::BinOp($1, OPERATION::assign, $3->coerce($1)); $$ = $1; }
		;

new_line: {}
		| T_NEW_LINE {}
		;
/*
 *	A declaration is given by a type of variable 
 *	and a list of variables
 */
declaration :	type variable_list { $$ = $2; }
				| type T_OPEN_BRACKETS size T_CLOSE_BRACKETS array_list { $$ = $5; }
// 				| T_DECL_FUNCTION type T_COLON function_list { $$ = $4; }
				;

/*
 *	A type of variable may be an integer, real or boolean
 */
type :	T_TYPE_INT { TYPE::lastType = TYPE::integer; }
		| T_TYPE_REAL { TYPE::lastType = TYPE::real; }
		| T_TYPE_BOOL { TYPE::lastType = TYPE::boolean; }
		;

/*
 *	Each list of variable can be a single word { creates a new instance of variableDeclaration and push the variable into the variable list}
 *	or a list of variables followed by a word { receives a new list and a variable, and push the the variable into the list }
 */
variable_list:	T_WORD { 
						 // cout<<"  Gram:variable_list:lastType "<<TYPE::maleName[TYPE::lastType]<<endl;
						 $$ = new AST::VariableDeclaration(TYPE::lastType);
						 // cout<<"  Gram:variable_list:lastType "<<TYPE::maleName[TYPE::lastType]<<endl;
						 $$->variables.push_back(symTab.newVariable($1, TYPE::lastType, KIND::variable, "0"));
						}
				| variable_list T_COMMA T_WORD { $$ = $1;
												 // cout<<"  parse:variable_list:lastType "<<TYPE::maleName[TYPE::lastType]<<endl;
												 $$->variables.push_back(symTab.newVariable($3, TYPE::lastType, KIND::variable, "0")); }
				;

/*
 *	creates a relation and assign a expression to a variable coming from target
 */
assignment: target T_ASSIGN expression { 
										// $1->printTree();
										// std::cout<<"\n  Gram:Assignment: "<< TYPE::maleName[$1->type] <<std::endl;
										// $3->printTree();
										// std::cout<<"\n  Gram:Assignment: "<< TYPE::maleName[$3->type] <<std::endl;
										$$ = new AST::BinOp($1, OPERATION::assign, $3->coerce($1)); }
			| target_array T_ASSIGN expression { $$ = new AST::BinOp($1, OPERATION::assign, $3->coerce($1)); }
			;

/*
 *	creates a new variable in the symbol table
 */
target: T_WORD { $$ = symTab.assignVariable($1, TYPE::lastType); }
		;

/*
 *	decalres a expression as being a variable, or a value, or an operation between two expressions, or a minus/negation operation of an expression
 *	for T_WORD, it uses a variable from the symbol table
 *	for the values, it creates a new instance of Value givin as parameters the value received and a <TYPE::type>
 */
expression:	T_WORD { $$ = symTab.useVariable($1); }
			| T_INT { $$ = new AST::Value($1, TYPE::integer); }
			| T_REAL { $$ = new AST::Value($1, TYPE::real); }
			| T_BOOL { $$ = new AST::Value($1, TYPE::boolean); }
			| expression T_PLUS expression { $$ = new AST::BinOp($1->coerce($3), OPERATION::plus, $3->coerce($1)); }
			| expression T_MINUS expression { $$ = new AST::BinOp($1->coerce($3), OPERATION::minus, $3->coerce($1)); }
			| expression T_TIMES expression { $$ = new AST::BinOp($1->coerce($3), OPERATION::times, $3->coerce($1)); }
			| expression T_DIVIDE expression { $$ = new AST::BinOp($1->coerce($3), OPERATION::divide, $3->coerce($1)); }
			| T_MINUS expression %prec U_NEGATIVE { $$ = new AST::UnOp(OPERATION::u_minus, $2); 
															 $$->type = $2->type;
															}
			| expression T_GREATER expression { $$ = new AST::BinOp($1->coerce($3), OPERATION::greater, $3->coerce($1)); }
			| expression T_GREATER_EQUALS expression { $$ = new AST::BinOp($1->coerce($3), OPERATION::greater_equals, $3->coerce($1)); }
			| expression T_SMALLER expression { $$ = new AST::BinOp($1->coerce($3), OPERATION::smaller, $3->coerce($1)); }
			| expression T_SMALLER_EQUALS expression { $$ = new AST::BinOp($1->coerce($3), OPERATION::smaller_equals, $3->coerce($1)); }
			| expression T_EQUALS expression { $$ = new AST::BinOp($1->coerce($3), OPERATION::equals, $3->coerce($1)); }
			| expression T_DIFFERENT expression { $$ = new AST::BinOp($1->coerce($3), OPERATION::different, $3->coerce($1)); }
			| expression T_AND expression { $$ = new AST::BinOp($1, OPERATION::and_op, $3); }
			| expression T_OR expression { $$ = new AST::BinOp($1, OPERATION::or_op, $3); }
			| T_NOT expression { $$ = new AST::UnOp(OPERATION::not_op, $2);
								 $$->type = $2->type;
								}
			| T_OPEN_PARENTHESIS expression T_CLOSE_PARENTHESIS { $$ = $2; }	
			| target_array { $$ = $1; }		
			;

/*
 *	only sets the size for the array
 */
size: T_INT {  Array::lastSize = $1; }
	  ;

/*
 *	the array variable can be only one variable or a list of variables 
 */
 
array_list: T_WORD { $$ = new AST::ArrayDeclaration(TYPE::lastType, Array::lastSize);
					 $$->variables.push_back(symTab.newVariable($1, TYPE::lastType, KIND::array, Array::lastSize));
					}
			| array_list T_COMMA T_WORD { $$ = $1;
			 							  $$->variables.push_back(symTab.newVariable($3, TYPE::lastType, KIND::array, Array::lastSize)); }
			;

/*
 *	target_array gets the array and the position to be assigned
 */
target_array: T_WORD T_OPEN_BRACKETS expression T_CLOSE_BRACKETS { $$ = symTab.assignVariable($1, TYPE::lastType); $$->size = $3; }
			 ;

/*
 *	declaration of scope
 */
scope: while_scope { $$ = $1; }
	 | if_scope { $$ = $1; }
	 | from_scope { $$ = $1; }
	 ;

/*
 *	declaration of while scope (loop)
 */
while_scope: T_WHILE T_OPEN_PARENTHESIS expression T_CLOSE_PARENTHESIS new_line T_OPEN_BRACES new_line block T_CLOSE_BRACES
					 { $$ = new AST::WhileBlock($3); 
					   if($8 != NULL) $$->lines.push_back($8); }
			;
			

/*
 *	declaration of from-until scope (loop)
 */
from_scope: T_FROM expression T_UNTIL expression new_line T_OPEN_BRACES new_line block T_CLOSE_BRACES
			{ $$ = new AST::FromTil_Block($2, $4);
			  if($8 != NULL) $$->lines.push_back($8);
			}
		  ;

/*
 *	declaration of if scope (conditional)
 */
if_scope: T_IF T_OPEN_PARENTHESIS expression T_CLOSE_PARENTHESIS new_line T_OPEN_BRACES new_line block T_CLOSE_BRACES 
					{ $$ = new AST::IfBlock($3);
					  if($8 != NULL) $$->thenLines.push_back($8); }
		 | T_IF T_OPEN_PARENTHESIS expression T_CLOSE_PARENTHESIS new_line T_OPEN_BRACES new_line block T_CLOSE_BRACES new_line T_ELSE new_line T_OPEN_BRACES new_line block T_CLOSE_BRACES 
		 			{ $$ = new AST::IfBlock($3);
					  if($8 != NULL) $$->thenLines.push_back($8); 
					  $$->hasElse = true;
					  if($15 != NULL) $$->elseLines.push_back($15); }
		 ;

// /*
//  *	list the function declaration, similar as it is done with the variable declaration
//  */
// function_list: T_WORD T_OPEN_PARENTHESIS T_CLOSE_PARENTHESIS { $$ = new AST::FunctionDeclaration(TYPE::lastType);
// 					 											$$->funcs.push_back(symTab.newVariable($1, TYPE::lastType)); }
// 				| T_WORD T_OPEN_PARENTHESIS parameters T_CLOSE_PARENTHESIS { $$ = new AST::FunctionDeclaration(TYPE::lastType);
// 					 											$$->funcs.push_back(symTab.newVariable($1, TYPE::lastType));
// 					 											$$->params.push_back($3); }
// 				;

// /*
//  *	gets the list of parameters - variables or arrays
//  */
// parameters: param { $$ = new AST::Param(TYPE::lastType); $$->paramList.push_back($1); }
// 			| parameters T_COMMA param { $1->paramList.push_back($3);}
// 			;

// /*
//  *	receives the declaration of new params
//  */
// param: type T_COLON variable_param { $$ = $3; }
// 	 | type T_OPEN_BRACKETS size T_CLOSE_BRACKETS T_COLON array_param { $$ = $6; }
// 	 ;

// /*
//  *	gets the name for a variable param
//  */
// variable_param:	T_WORD { $$ = new AST::VariableDeclaration(TYPE::lastType);
// 						 $$->variables.push_back(symTab.newVariable($1, TYPE::lastType)); }
// 				;

// /*
//  *	gets the name for an array param
//  */
// array_param: T_WORD { $$ = new AST::ArrayDeclaration(TYPE::lastType, Array::lastSize);
// 					 $$->variables.push_back(symTab.newVariable($1, TYPE::lastType)); }
// 			;

// /*
//  *	the definition of functions still need to be implemented
//  */
// def_func: T_DEF_FUNCTION type T_COLON func_signature new_line func_body T_END_DEF { $$ = new FunctionDefinition(TYPE::lastType, $4);
// 																							$$->lines.push_back($6); 
// 																						}
// func_signature: function_list { $$ = $1; $$->isDef = true;}
// 				;

// func_body: lines return { $$ = new AST::FunctionBody(); $$->lines.push_back($1); $$->lines.push_back($2);}
// 		 | func_body lines return { $1->lines.push_back($2); $1->lines.push_back($3);}
// 		 | return { $$ = new AST::FunctionBody(); $$->lines.push_back($1); }
// 		 ;

// return: T_RETURN expression T_SEMICOLON new_line { $$ = new AST::FunctionReturn($2); };

// def_type: T_DEF_TYPE T_COLON variable_list new_line type_body T_END_DEF { $$ = new AST::TypeDef($3); 
// 																			$$->nodes.push_back($5); }
// 		;

// type_body: declaration T_SEMICOLON new_line { $$ = new AST::TypeBody(); $$->lines.push_back($1); }
// 		 | type_body declaration T_SEMICOLON new_line { $1->lines.push_back($2); }
// 		 ;

%%

