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
			// std::string value;
			bool initialized;
			Symbol( TYPE::Type newType, KIND::Kind newKind, std::string newLengh, bool init ) 
					: type(newType), kind(newKind), lengh(newLengh), initialized(init) { 
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
			SymbolTable* parent;
			SymbolTable() : parent(nullptr){}
			SymbolTable(SymbolTable* parentTable) : parent(parentTable){}
			bool checkId( std::string id ) { 
				if(entryList.find(id) != entryList.end())
         			return true;
       			if(parent != NULL)
          			return parent->checkId(id);
        		return false;
			} // @return true if variable was defined
			void addSymbol(std::string id, Symbol newSymbol) { entryList[id] = newSymbol; }
			AST::Node* newVariable( std::string id, TYPE::Type type, KIND::Kind kind, std::string lengh);
			AST::Node* assignVariable(std::string id, TYPE::Type type);
			AST::Node* useVariable(std::string id);
			Symbol* getSymbol(std::string id){
				if(entryList.find(id) != entryList.end())
					return &entryList[id];
				if( parent != NULL )
					return parent->getSymbol(id);
				return nullptr;
			}
	};
}
