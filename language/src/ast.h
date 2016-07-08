/*
 *	Abstract Syntax Tree (AST)
 *	references: 
 *	 - llpilla/compiler_examples/allen (github)
 *	 - Aho, Alfred, et al - Compilers: principles, techniques, and tools - 2nd ed. 2007 Pearson Education
 *	 - Levine, John - Flex & Bison - Unix text processing tools - 2009 O'Reilly Media
 */
#pragma once

#include <iostream>
#include <vector>
#include "staff.h"


using namespace std;

extern void yyerrer(const char *s, ...);
namespace AST {

	class Node;

	/*
	 *	List of ASTs
	 */
	typedef std::vector<Node*> NodeList;
	typedef std::vector<Node*> ParamList;

	/*
	 *	@class Node to define a node of the syntax tree
	 *	@param TYPE::Type form the staff.h to indicate the type of the node ( integer, real, boolean )
	 *	@method printTree is used to print the tree	 @return void
	 *	@method coerce  @return Node*	coerces this node to real if needed
	 *	@method needCoersion  @return bool 	checks if this node needs coercion
	 */
	class Node {
		public:
			TYPE::Type type = TYPE::unknown;
			Node* size;
			virtual ~Node() { }
			Node (TYPE::Type type) : type(type) { }
			Node () { }
			virtual void printTree() { }
			Node* coerce(Node* node);
			bool needCoersion(TYPE::Type a, TYPE::Type b);
			bool isParam = false;
			bool isDef = false;
			bool isComplex = false;
			bool error = false;
			// ParamList params;
			NodeList paramList;
			KIND::Kind kind;
	};

	/*
	 *	@class Block to work with a block structure
	 *	@attribute NodeList (list of lines)
	 *	@param none
	 *	@method printTree  @return void
	 */
	class Block : public Node {
		public:
			NodeList lines;
			Block() { }
			void printTree();
	};

	/*
	 *	@class Value to receive numerical values
	 *	@param std::string, TYPE::Type (type is assigned to the Node)
	 *	@method printTree  @return void
	 */
	class Value : public Node {
		public:
			std::string value;
			TYPE::Type type;
			Value(std::string value, TYPE::Type newType) : value(value), type(newType), Node(newType) { 
				// std::cout<<"value: "<<value<<std::endl;
				// std::cout<<"type: "<<TYPE::maleName[this->type]<<std::endl;
				// std::cout<<"newType: "<<TYPE::maleName[newType]<<std::endl;
			}
			void printTree();
	};

	/*
	 *	@class BinOp to work with binary operation ( +, *, =, ~=, ... )
	 *	@param Node, OPERATION::Operation, Node
	 *	@method printTree  @return void
	 */
	class BinOp : public Node {
		public:
			OPERATION::Operation op;
			Node *left;
			Node *right;
			BinOp(Node *newLeft, OPERATION::Operation op, Node *newRight) {
				// std::cout<<"op "<< OPERATION::name[op]<<std::endl;
				switch(op){
					case OPERATION::assign:
						assign(newLeft, op, newRight);
					break;
					case OPERATION::plus:
					case OPERATION::minus:
					case OPERATION::times:
					case OPERATION::divide:
						math(newLeft, op, newRight);
					break;
					default:
						this->op = op;
						this->left = newLeft;
						this->right = newRight;
					break;
				}
			}
			void printTree();
			void assign(Node *newLeft, OPERATION::Operation op, Node *newRight);
			void validateAndAssign(Node *newLeft, OPERATION::Operation op, Node *newRight);
			void math(Node *newLeft, OPERATION::Operation op, Node *newRight);
			void coerceToInteger(Node *newLeft, Node *newRight);

	};

	/*
	 *	@class UnOp to work with unary operation ( -, ~, := )
	 *	@param OPERATION::Operation, Node
	 *	@method printTree  @return void
	 */
	class UnOp : public Node {
		public:
			OPERATION::Operation op;
			Node *node;
			TYPE::Type type;
			UnOp(OPERATION::Operation newOp, Node* newNode) : node(newNode), op(newOp), type(newNode->type) {
				// std::cout<<"  Unop:Type: "<< TYPE::maleName[this->type] <<std::endl;
				// std::cout<<"  UnOp:NodeType: "<< TYPE::maleName[this->node->type] <<std::endl;
				// std::cout<<"  Unop:Op: "<< OPERATION::name[this->op] <<std::endl;
				checkType(newNode->type, newOp);
				// TYPE::getUnType(node->type, op); 
			}
			void checkType(TYPE::Type type, OPERATION::Operation op);
			void printTree();
	};

	/*
	 *	@class Word to work with strings (declare variables)
	 *	@param std::string, TYPE::Type (type is assigned to the Node)
	 *	@method printTree  @return void
	 */
	class Word : public Node {
		public:
			std::string word;
			TYPE::Type type;
			KIND::Kind kind;
			std::string lengh;
			Word(std::string word, TYPE::Type newType, KIND::Kind newKind, std::string newLengh) 
				: word(word), type(newType), kind(newKind), Node(newType), lengh(newLengh) { }
			void printTree();
	};

	/*
	 *	@class VariableDeclaration to receive numerical values
	 *	@attribute NodeList (list of variables), TYPE::Type
	 *	@param TYPE::Type (type is assigned to the Node)
	 *	@method printTree  @return void
	 */
	 class VariableDeclaration : public Node {
	 	public:
	 		TYPE::Type type;
	 		NodeList variables;
	 		VariableDeclaration (TYPE::Type type) : type(type), Node(type){
	 			// std::cout<< "declaravar" <<std::endl;
	 			// std::cout<< "tipo "<< type <<std::endl;
	 		}
	 		void printTree();
	 };

	 /*
	 *	@class ArrayDeclaration
	 *	@attribute NodeList (list of variables)
	 *	@param TYPE::Type (type is assigned to the Node), std::string (size of the array)
	 *	@method printTree  @return void
	 */
	 class ArrayDeclaration : public Node {
	 	public:
	 		std::string size;
	 		TYPE::Type type;
	 		NodeList variables;
	 		ArrayDeclaration (TYPE::Type type, std::string size) : type(type), size(size), Node(type) { }
	 		void printTree();
	 };

	 /*
	  *	@class Coercion
	  *	@attribute Node
	  *	@param Node
	  *	@method printTree  @return void
	  *	it sets the node type to <Type::real>
	  */
	 class Coercion : public Node {
	 	public:
	 		Node* node;
	 		Coercion(Node* node) : node(node), Node(TYPE::real) { }
	 		void printTree();
	 };

	 /*
	 *	@class FunctionDeclaration
	 *	@attribute NodeList (list of functions)
	 *	@param TYPE::Type (type is assigned to the Node)
	 *	@method printTree  @return void
	 */
	 class FunctionDeclaration : public Node {
	 	public:
	 		TYPE::Type type;
	 		NodeList funcs;
	 		ParamList params;
	 		FunctionDeclaration (TYPE::Type type) : type(type), Node(type) { }
	 		void printTree();
	 };

	 /*
	 *	@class FunctionDeclaration
	 *	@attribute NodeList (list of functions)
	 *	@param TYPE::Type (type is assigned to the Node)
	 *	@method printTree  @return void
	 */
	 class Param : public Node {
	 	public:
	 		TYPE::Type type;
	 		// NodeList paramList;
	 		
	 		Param (TYPE::Type type) : type(type), Node(type) { }
	 		void printTree();
	 };

	 /*
	 *	@class FunctionDeclaration
	 *	@attribute NodeList (list of functions)
	 *	@param TYPE::Type (type is assigned to the Node) , Node*
	 *	@method printTree  @return void
	 */
	 class FunctionDefinition : public Node {
	 	public:
	 		TYPE::Type type;
	 		NodeList lines;
	 		NodeList funcs;
	 		ParamList params;

	 		Node *signature;
	 		FunctionDefinition (TYPE::Type type/*, Node *signature*/) : type(type), Node(type)/*, signature(signature)*/ { }
	 		void printTree();
	 };

	 /*
	 *	@class FunctionReturn
	 *	@attribute Node*
	 *	@method printTree  @return void
	 *	@param Node*
	 *	stores and returns the return value of the function
	 */
	 class FunctionReturn : public Node {
	 	public:
	 		Node *value;
	 		FunctionReturn ( Node *value) : value(value) { }
	 		void printTree();
	 };

	 /*
	 *	@class IfBlock to work with a if-then-else structure
	 *	@attribute NodeList (list of lines), Node*, bool
	 *	@param Node*
	 *	@method printTree  @return void
	 */
	class IfBlock : public Node {
		public:
			NodeList thenLines;
			NodeList elseLines;
			Node* condition;
			bool hasElse = false;
			IfBlock(Node* condition) : condition(condition) { }
			void printTree();
	};

	/*
	 *	@class WhileBlock to work with a while structure
	 *	@attribute NodeList (list of lines), Node*
	 *	@param Node*
	 *	@method printTree  @return void
	 */
	class WhileBlock : public Node {
		public:
			NodeList lines;
			Node* condition;
			WhileBlock(Node* condition) : condition(condition) { }
			void printTree();
	};

	/*
	 *	@class FunctionBody to work with a function block structure
	 *	@attribute NodeList (list of lines)
	 *	@method printTree  @return void
	 */
	class FunctionBody : public Node {
		public:
			NodeList lines;
			FunctionBody() { }
			void printTree();
	};

	/*
	 *	@class TypeDef to work with a while structure
	 *	@attribute NodeList (list of Nodes - componentes), Node*
	 *	@param Node*
	 *	@method printTree  @return void
	 */
	class TypeDef : public Node {
		public:
			NodeList nodes;
			Node *name;
			TypeDef(Node *name) : name(name) { }
			void printTree();
	};

	class FromTil_Block : public Node {
		public:
			NodeList lines;
			Node* fromExpr;
			Node* tilExpr;
			FromTil_Block(Node* fromExpr, Node* tilExpr) : fromExpr(fromExpr), tilExpr(tilExpr) {
				if(fromExpr->type != TYPE::integer){
					error = true;
					std::cout<<"Erro:\n - semântico: o valor de ";
					fromExpr->printTree();
					std::cout<<" deve ser do tipo "<< TYPE::maleName[TYPE::integer]<< ", mas recebeu "<< TYPE::maleName[fromExpr->type]<< "." <<endl;
				}
				if(tilExpr->type != TYPE::integer){
					error = true;
					std::cout<<"Erro:\n - semântico: o valor de ";
					tilExpr->printTree();
					std::cout<<" deve ser do tipo "<< TYPE::maleName[TYPE::integer]<< ", mas recebeu "<< TYPE::maleName[tilExpr->type]<< "." <<endl;
				}
			}
			void printTree();
	};
}

namespace Array{

	/*
	 *	stores the declared size of the array
	 */
	static std::string lastSize;

}
