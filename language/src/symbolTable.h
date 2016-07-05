/*
 *	Symbol Table
 *	references: 
 *	 - llpilla/compiler_examples/allen (github)
 *	 - Aho, Alfred, et al - Compilers: principles, techniques, and tools - 2nd ed. 2007 Pearson Education
 *	 - Levine, John - Flex & Bison - Unix text processing tools - 2009 O'Reilly Media
 */
#pragma once

#include <map>
#include "ast.h"
#include "staff.h"

extern void yyerror(const char* s, ...);

namespace ST {
	class Symbol;

	// enum Kind { variable, array, unknown };

	typedef std::map<std::string, Symbol> SymbolList; // set of symbols

	/*
	 *	@class Symbol to define a symbol 
	 *	@param TYPE::Type form the staff.h to indicate the type of the node ( integer, real, boolean )
	 *	@param Kind (variable)
	 *	@param initialized to indicate wether the variable was initialized or not
	 */
	class Symbol {
		public:
			TYPE::Type type;
			KIND::Kind kind;
			std::string lengh = "0";
			std::string value;
			bool initialized;
			Symbol( TYPE::Type newType, KIND::Kind newKind, std::string newLengh, bool init ) 
					: type(newType), kind(newKind), lengh(newLengh), initialized(init) { 
					// std::cout << "symbol: "<<newType<<endl;
					}
			Symbol( ) {type = TYPE::unknown; kind = KIND::unknown; initialized = false; lengh = "0"; }
	};

	/*
	 *	@class SymbolTable to create a symbol table
	 *	@param SymbolList - map of symbols
	 *	@method checkId @param std::string @return boolean  checks if a symbol is already in the enryList map
	 *	@method addSymbol @param std::string, Symbol @return void  adds a new symbol in the entryList map
	 *	@method newVariable @param std::string TYPE::Type @return AST::Node  creates a new node for the variable
	 *	@method assignVariable @param std::string @return AST::Node  makes an assignment to the given variable
	 *	@method useVariable @param std:string @return AST::Node  retrieve a variable from symbol of the entryList
	 */
	class SymbolTable {
		public:
			SymbolList entryList;
			SymbolTable(){}
			bool checkId( std::string id ) { return entryList.find(id) != entryList.end(); } // @return true if variable was defined
			void addSymbol(std::string id, Symbol newSymbol) { entryList[id] = newSymbol; }
			AST::Node* newVariable( std::string id, TYPE::Type type, KIND::Kind kind, std::string lengh);
			AST::Node* assignVariable(std::string id, TYPE::Type type);
			AST::Node* useVariable(std::string id);
	};
}
