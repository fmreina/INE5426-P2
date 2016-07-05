/*
 *	Main
 *	references: 
 *	 - llpilla/compiler_examples/allen (github)
 *	 - Aho, Alfred, et al - Compilers: principles, techniques, and tools - 2nd ed. 2007 Pearson Education
 *	 - Levine, John - Flex & Bison - Unix text processing tools - 2009 O'Reilly Media
 */
#include <iostream>
#include "ast.h"
extern int yydebug;
extern AST::Block* programRoot; // set on bison file
extern int yyparse();

int main (int argc, char **argv){
	yydebug = 0;
	yyparse();					// parses the data
	programRoot->printTree();	// prints the AST
	return 0;
}
