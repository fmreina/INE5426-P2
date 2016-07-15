// Parser - references as shown on README.md
%{
	#include "ast.h"
	#include "symbolTable.h"
	using namespace std;
	using namespace AST;
	
	AST::Block* programRoot; /* root node of program AST */
	ST::SymbolTable symTab; /* global symbol table -  this is visible in the global scope */
	ST::SymbolTable symTabAux;
	ST::SymbolTable symTabHist;

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
	AST::FunctionDeclaration *fun;
	AST::FunctionDefinition *func_def;
	// AST::FunctionReturn *func_return;
	AST::FunctionBody *body;
	void* novalue;
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
%token T_TYPE_VOID
%token T_TYPE_INT
%token T_TYPE_REAL
%token T_TYPE_BOOL
%token T_WHILE
%token T_IF
%token T_THEN
%token T_ELSE
%token T_FROM
%token T_UNTIL
%token T_DEF_FUNCTION
%token T_DECL_FUNCTION
%token T_RETURN

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
%type <novalue> new_line
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
%type <fun> funct
%type <node> parameters
%type <body> body
%type <node> param
%type <var> variable_param
%type <arr> array_param
%type <func_def> def_func
// %type <func_return> return
%type <node> new_scope;
%type <node> end_scope;
%type <novalue> startcode
 
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
program : startcode block { programRoot = $2;
							// if($2->lines.back()->code != NULL){
								IR::codeGenEnd($2->lines.back()->code);	
							// } 
						 }
 		;
 	
/*
 *	Each group of lines can be a single line or 
 *	a group of lines followed by a single line
 */
block :	line { $$ = new AST::Block(); if ($1 != NULL) $$->lines.push_back($1); }
 		| block line { if($2 != NULL) $1->lines.push_back($2); }
 		;

/* startcode: Only sets up the code generation environment */
startcode : { IR::codeGenSetup(); }
          ;

/*
 *	A line may be a declaration or an assignment
 */ 		
line :	declaration T_SEMICOLON new_line { $$ = $1; }
		| assignment T_SEMICOLON new_line
		| line new_line 
		| scope new_line
		| funct new_line{ $$ = $1; }
		| def_func new_line { $$ = $1; }
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
				;

/*
 *	A type of variable may be an integer, real or boolean
 */
type :	T_TYPE_INT { TYPE::lastType = TYPE::integer; }
		| T_TYPE_REAL { TYPE::lastType = TYPE::real; }
		| T_TYPE_BOOL { TYPE::lastType = TYPE::boolean; }
		| T_TYPE_VOID { TYPE::lastType = TYPE::vazio; }
		;

/*
 *	Each list of variable can be a single word { creates a new instance of variableDeclaration and push the variable into the variable list}
 *	or a list of variables followed by a word { receives a new list and a variable, and push the the variable into the list }
 */
variable_list:	T_WORD { $$ = new AST::VariableDeclaration(TYPE::lastType);
						 $$->variables.push_back(symTab.newVariable($1, TYPE::lastType, KIND::variable, "0"));
						}
				| variable_list T_COMMA T_WORD { $$ = $1;
												  $$->variables.push_back(symTab.newVariable($3, TYPE::lastType, KIND::variable, "0")); }
				;

/*
 *	creates a relation and assign a expression to a variable coming from target
 */
assignment: target T_ASSIGN expression { $$ = new AST::BinOp($1, OPERATION::assign, $3->coerce($1)); }
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
			 							  $$->variables.push_back(symTab.newVariable($3, TYPE::lastType, KIND::array, Array::lastSize)); 
			 							}
			;

/*
 *	target_array gets the array and the position to be assigned
 */
target_array: T_WORD T_OPEN_BRACKETS expression T_CLOSE_BRACKETS { $$ = symTab.assignVariable($1, TYPE::lastType); $$->size = $3; }
			;

/*
 *	declaration of scope
 */
scope: /*while_scope{ $$ = $1; }*/
	   while_scope end_scope { $$ = $1; }
	 | if_scope { $$ = $1; }
	 | from_scope { $$ = $1; }
	 ;

new_scope: { symTab = new ST::SymbolTable(symTab); };

end_scope: { if(symTab.parent != nullptr) symTab = symTab.parent; };

/*
 *	declaration of while scope (loop)
 */
while_scope: T_WHILE T_OPEN_PARENTHESIS expression T_CLOSE_PARENTHESIS new_line T_OPEN_BRACES new_line new_scope block T_CLOSE_BRACES
					 { $$ = new AST::WhileBlock($3); 
					   if($9 != NULL) $$->lines.push_back($9); }
			;
			

/*
 *	declaration of from-until scope (loop)
 */
from_scope: T_FROM expression T_UNTIL expression new_line T_OPEN_BRACES new_line new_scope block end_scope T_CLOSE_BRACES
			{ $$ = new AST::FromTil_Block($2, $4);
			  if($9 != NULL) $$->lines.push_back($9);
			}
		  | T_FROM expression T_UNTIL expression new_line T_OPEN_BRACES new_line T_CLOSE_BRACES
			{ $$ = new AST::FromTil_Block($2, $4);
			}
		  ;

/*
 *	declaration of if scope (conditional)
 */
if_scope: T_IF T_OPEN_PARENTHESIS expression T_CLOSE_PARENTHESIS new_line T_OPEN_BRACES new_line new_scope block end_scope T_CLOSE_BRACES 
					{ $$ = new AST::IfBlock($3);
					  if($9 != NULL) $$->thenLines.push_back($9); }
		 | T_IF T_OPEN_PARENTHESIS expression T_CLOSE_PARENTHESIS new_line T_OPEN_BRACES new_line new_scope block end_scope T_CLOSE_BRACES new_line T_ELSE new_line T_OPEN_BRACES new_line new_scope block end_scope T_CLOSE_BRACES 
		 			{ $$ = new AST::IfBlock($3);
					  if($9 != NULL) $$->thenLines.push_back($9); 
					  $$->hasElse = true;
					  if($18 != NULL) $$->elseLines.push_back($18); }
		 ;


/*
 *	list the function declaration, similar as it is done with the variable declaration
 */
funct: type T_WORD T_OPEN_PARENTHESIS parameters T_CLOSE_PARENTHESIS T_SEMICOLON  // declaration
				{ $$ = new AST::FunctionDeclaration(TYPE::lastType);
				  $$->funcs.push_back(symTab.newVariable($2, TYPE::lastType, KIND::function, "0"));
				  if($4 != NULL) $$->params.push_back($4);
				}
	;

def_func: type T_WORD T_OPEN_PARENTHESIS parameters T_CLOSE_PARENTHESIS T_OPEN_BRACES body T_CLOSE_BRACES // definition
				{ $$ = new FunctionDefinition(TYPE::lastType);
				  $$->funcs.push_back(symTab.newVariable($2, TYPE::lastType, KIND::function, "0"));
	  			  if($4 != NULL) $$->params.push_back($4);
	  			  if($7 != NULL) $$->lines.push_back($7);
	  			}
		;

body: { $$ = NULL; }
	| body new_line {}
	//| T_RETURN expression T_SEMICOLON { /*$$-> lines.push_back(new AST::FunctionReturn($2));*/ }
	// | block new_line return new_line { $$ = new AST::FunctionBody(); 
	// 								   if($1 != NULL) $$->lines.push_back($1); 
	// 								   $$->lines.push_back($3);
	// 								 }
	// | body block new_line return new_line { //if($1 != NULL) {
	// 											$1->lines.push_back($2); 
	// 											$1->lines.push_back($4);
	// 										//}
	// 									}
	// | return { $$ = new AST::FunctionBody(); 
	// 		   if($1 != NULL) 
	// 		   	$$->lines.push_back($1); 
	// 		 }
	;

// return: T_RETURN expression T_SEMICOLON new_line { $$ = new AST::FunctionReturn($2); };

// func_body: new_line {}
// 		 | block new_line return new_line { $$ = new AST::FunctionBody(); $$->lines.push_back($1); $$->lines.push_back($3);}
// 		 | func_body block new_line return new_line { $1->lines.push_back($2); $1->lines.push_back($4);}
// 		 | return { $$ = new AST::FunctionBody(); $$->lines.push_back($1); }
// 		 ;



parameters: { $$ = NULL; }
		  | parameters new_line {}
  		  | param { $$ = new AST::Param(TYPE::lastType); $$->paramList.push_back($1); }
		  | parameters T_COMMA param { $1->paramList.push_back($3); }
		  ;
/*
 *	receives the declaration of new params
 */
param: type variable_param { $$ = $2; }
	 | type T_OPEN_BRACKETS size T_CLOSE_BRACKETS  array_param { $$ = $5; }
	 ;

/*
 *	gets the name for a variable param
 */
variable_param:	T_WORD { $$ = new AST::VariableDeclaration(TYPE::lastType);
						 $$->variables.push_back(symTab.newVariable($1, TYPE::lastType, KIND::variable, "0")); }
				;

/*
 *	gets the name for an array param
 */
array_param: T_WORD { $$ = new AST::ArrayDeclaration(TYPE::lastType, Array::lastSize);
					 $$->variables.push_back(symTab.newVariable($1, TYPE::lastType, KIND::array, "0")); }
			;

%%

