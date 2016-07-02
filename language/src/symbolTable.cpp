/*
 *	Symbol Table
 *	references: 
 *	 - llpilla/compiler_examples/allen (github)
 *	 - Aho, Alfred, et al - Compilers: principles, techniques, and tools - 2nd ed. 2007 Pearson Education
 *	 - Levine, John - Flex & Bison - Unix text processing tools - 2009 O'Reilly Media
 */
#include "symbolTable.h"

using namespace ST;

extern SymbolTable symTab;

/*
 *	adds a new symbol in the symbolTable
 *	returns a new node
 */
AST::Node* SymbolTable::newVariable( std::string id, TYPE::Type type){
	// std::cout<<"new variable: "<<type<<std::endl;
	if( checkId(id) ) yyerror("Redefinicão de variável! %s\n", id.c_str());
	else {
		Symbol entry(type, variable, false); 
		addSymbol( id, entry );
	}

	return new AST::Word( id, type );
}

/*
 *	initializes a variable declared previously
 */
AST::Node* SymbolTable::assignVariable( std::string id, TYPE::Type type ){
	// TYPE::Type type;
	if( !checkId(id) ) yyerror("Variável ainda não foi definida! %s\n", id.c_str());
	else{
		// type = entryList[id].type;
		entryList[id].initialized = true;
		return new AST::Word( id, type);
	}
	return 0;
}

/*
 *	retreive a variable previously declared and initialized
 */
AST::Node* SymbolTable::useVariable( std::string id ){
	// std::cout<< "symbolTable" <<std::endl;
	TYPE::Type type;
	if( !checkId(id) ) yyerror("Variável ainda não foi definida! %s\n", id.c_str());
	else{
		TYPE::Type type = entryList[id].type;
		if( !entryList[id].initialized ) yyerror("Variável ainda não foi inicializada! %s\n", id.c_str());
		// std::cout<<"useVariable "<< id << " : " << type <<std::endl;
		// std::cout<<"entrylist.type: "<<entryList[id].type<<endl;
		return new AST::Word( id, type );
	}
	return 0;
}
