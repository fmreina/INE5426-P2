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
AST::Node* SymbolTable::newVariable( std::string id, TYPE::Type type, KIND::Kind kind, std::string lengh){
	if( checkId(id) ) yyerror("Redefinicão de variável! %s\n", id.c_str());
	else {
		Symbol entry(type, kind, lengh, false); 
		addSymbol( id, entry );
	}

	AST::Node* node = new AST::Word( id, type, kind, lengh );
	// node->kind = kind;
	return node;
}

/*
 *	initializes a variable declared previously
 */
AST::Node* SymbolTable::assignVariable( std::string id, TYPE::Type type ){
	if( !checkId(id) ) yyerror("Variável ainda não foi definida! %s\n", id.c_str());
	else{
		KIND::Kind kind;
		std::string lengh;

		Symbol* symbol = getSymbol(id);
		if(symbol != nullptr){
			type = symbol->type;
			kind = symbol->kind;
			lengh = symbol->lengh;
			symbol->initialized = true;
		}else{
			type = entryList[id].type;
			kind = entryList[id].kind;
			lengh = entryList[id].lengh;
			entryList[id].initialized = true;
		}

		// entryList[id].initialized = true;
		// KIND::Kind kind = entryList[id].kind;
		// std::string lengh = entryList[id].lengh;
		// entryList[id].value = "valueOf("+id+")"; // TODO: to complete: receive the value and set to the symbol
		AST::Node* node = new AST::Word( id, type, kind, lengh );
		// node->kind = kind;
		// cout<<"NodeId: "<< id <<endl;
		// cout<<"NodeType: "<< type <<endl;
		// cout<<"NodeKind: "<< kind <<endl;
		// cout<<"NodeLengh: "<< lengh <<endl;
		return node;
	}
	return 0;
}

/*
 *	retreive a variable previously declared and initialized
 */
AST::Node* SymbolTable::useVariable( std::string id ){
	TYPE::Type type;
	if( !checkId(id) ) yyerror("Variável ainda não foi definida! %s\n", id.c_str());
	else{
		TYPE::Type type;
		KIND::Kind kind;
		std::string lengh;
		bool initialized = false;

		Symbol* symbol = getSymbol(id);
		if(symbol != nullptr){
			type = symbol->type;
			kind = symbol->kind;
			lengh = symbol->lengh;
			initialized = symbol->initialized;
		}else{
			type = entryList[id].type;
			kind = entryList[id].kind;
			lengh = entryList[id].lengh;
			initialized = entryList[id].initialized;
		}
		// TYPE::Type type = entryList[id].type;
		// KIND::Kind kind = entryList[id].kind;
		// std::string lengh = entryList[id].lengh;
		if( !initialized ) yyerror("Variável ainda não foi inicializada! %s\n", id.c_str());
		AST::Node* node = new AST::Word( id, type, kind, lengh );
		// node->kind = kind;
		// cout<<"NodeId: "<< id <<endl;
		// cout<<"NodeType: "<< type <<endl;
		// cout<<"NodeKind: "<< kind <<endl;
		// cout<<"NodeLengh: "<< lengh <<endl;
		return node;
	}
	return 0;
}
