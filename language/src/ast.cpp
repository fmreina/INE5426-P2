/*
 *	Abstract Syntax Tree (AST)
 *	references: 
 *	 - llpilla/compiler_examples/allen (github)
 *	 - Aho, Alfred, et al - Compilers: principles, techniques, and tools - 2nd ed. 2007 Pearson Education
 *	 - Levine, John - Flex & Bison - Unix text processing tools - 2009 O'Reilly Media
 */

#include "ast.h"
#include "symbolTable.h"

using namespace AST;

extern ST::SymbolTable symTab;

/*
 *	prints each line stored in the list lines
 *	example:	line <break line>
 *				line <break line>
 *				line <break line>
 */
void Block::printTree(){
	for (Node* line: lines) {
		line->printTree();
		std::cout << std::endl;
	}
}

/*
 *	prints the binary operation in the following format
 *	( <Node> (<OPERATION::Operation> <TYPE::Type>) <Node>)
 *	example: ( 2 (sum real) 3)
 */
void BinOp::printTree(){
	switch(op){
		case OPERATION::assign:

			// check if it's an array or not
			if(left->size == NULL){
				std::cout << "Atribuicão de valor para ";
				left->printTree();
				std::cout << ": ";
			}else if(left->size != NULL){
				std::cout << "Atribuicão de valor para arranjo "<<  TYPE::maleName[type] << " ";
				left->printTree();
				std::cout << " {+indice: ";
				left->size->printTree();
				std::cout << "}: ";
			}

			// check if it's an array or not
			if(right->size == NULL){
				right->printTree();	
			}else if(right->size != NULL){
				std::cout << "arranjo "<<  TYPE::maleName[type] << " ";
				right->printTree();
				std::cout << " {+indice: ";
				right->size->printTree();
				std::cout << "}: ";
			}
			break;
		default:
			std::cout << "(";
			
			// check if it's an array or not
			if(left->size == NULL){
				left->printTree();
			} else if(left->size != NULL){ 
				std::cout << "arranjo "<<  TYPE::maleName[type] << " ";
				left->printTree();
				std::cout << " {+indice: ";
				left->size->printTree();
				std::cout << "}";
			}
			std::cout << " (" << OPERATION::name[op] << " ";
			
			if(OPERATION::maleGender[op]){
				std::cout<< TYPE::maleName[type] << ") ";	
			} else {
				std::cout<< TYPE::femaleName[type] << ") ";	
			}
			
			// check if it's an array or not
			if(right->size == NULL){
				right->printTree();
			} else if(right->size != NULL){
				std::cout << "arranjo "<<  TYPE::maleName[type] << " ";
				right->printTree();
				std::cout << " {+indice: ";
				right->size->printTree();
				std::cout << "}";
			}
			
			std::cout << ")";
			break;
	}
	return;
}

/*
 *	prints the unary operation in the following format
 *	((<OPERATION::Operation> <TYPE::Type>) <Node>)
 *	example: ((negation boolean) true)
 */
void UnOp::printTree(){
	switch(op){
		case OPERATION::parenthesis:
			std::cout << "(abre parenteses) ";
            // node->printTree();
			// check if it's an array or not
            if(node->size == NULL){
				node->printTree();
			} else if(node->size != NULL){
				std::cout << "arranjo "<<  TYPE::maleName[type] << " ";
				node->printTree();
				std::cout << " {+indice: ";
				node->size->printTree();
				std::cout << "}";
			}
            
            std::cout << " (fecha parenteses)";
			break;
		default:
			std::cout << "(";
			std::cout << "(" << OPERATION::name[op] << " ";
			if(OPERATION::maleGender[op]){
				std::cout<< TYPE::maleName[type] << ") ";	
			} else{
				std::cout<< TYPE::femaleName[type] << ") ";	
			}
			// node->printTree();
			// check if it's an array or not
            if(node->size == NULL){
				node->printTree();
			} else if(node->size != NULL){
				std::cout << "arranjo "<<  TYPE::maleName[type] << " ";
				node->printTree();
				std::cout << " {+indice: ";
				node->size->printTree();
				std::cout << "}";
			}
			std::cout << ")";
			break;
	}
	return;
}

/*
 *	prints the variable in the following format (using portuguese)
 *	variável <TYPE::Type> <std::string>
 *	exemple: variável boolean flag
 */
void Word::printTree(){
	if(size == NULL){
		if(type==TYPE::integer && type==TYPE::real && type==TYPE::boolean){
			std::cout << "variável " << TYPE::femaleName[type] << " " << word;
		} 
		std::cout << "variável " << word;
	}
	else{
		std::cout << word;	
	}
	return;
}

/*
 *	prints the value in the following format (using portuguese)
 *	valor <TYPE::Type> <std::string>
 *	exemple: valor boolean TRUE
 */
 void Value::printTree(){
 	std::cout << "valor " << TYPE::maleName[type] << " " << value;
 	return;
 }

/*
 *	prints the value declaration in the following format (using portuguese)
 *	Declaracão de variável <TYPE::Type> <std::string>: <list of variables>
 *	exemple: Declaracão de variável <inteira>: <variable>
 */
 void VariableDeclaration::printTree(){
 	if(isParam){
 		std::cout << TYPE::maleName[type] << ": ";
 	}else if(!isComplex){
	 	std::cout << "Declaracão de variável "<< TYPE::femaleName[type] << ": ";
 	}
 	for( auto var = variables.begin(); var != variables.end(); var ++){
 		std::cout << dynamic_cast<Word *>(*var)->word;
 		if(next(var) != variables.end())
 			std::cout << ", ";
 	}
 }

/*
 *	Method to make the coercion from integer to real when needed
 */
 Node* Node::coerce(Node* node){
 	if(this->needCoersion(this->type, node->type)){
 		type = TYPE::real;
 		return new AST::Coercion(this);
 	}
 	type = TYPE::integer;
 	return this;
 }

/*
 *	check if needs to make a coercion. If this->node is integer and the other is real return true.
 */
 bool Node::needCoersion(TYPE::Type a, TYPE::Type b){
 	return(a == TYPE::integer && b == TYPE::real);
 }

/*
 *	prints the array declaration in the following format (using portuguese)
 *	Declaracão de arranjo <TYPE::Type> de tamanho <std::string>: <std::string>
 *	exemple: Declaracão de arranjo <inteiro> de tamanho <10>: <arr>
 */
 void ArrayDeclaration::printTree(){
 	if(isParam){
 		std::cout << "arranjo " << TYPE::maleName[type] << " de tamanho "<< size <<": ";
 	}else{
 		std::cout << "Declaracão de arranjo " << TYPE::maleName[type] << " de tamanho "<< size <<": ";
 	}
 	for( auto var = variables.begin(); var != variables.end(); var ++){
 		std::cout << dynamic_cast<Word *>(*var)->word;
 		if(next(var) != variables.end())
 			std::cout << ", ";
 	}
 }

/*
 *	prints the coercion to real when needed
 */
 void Coercion::printTree(){
 	node->printTree();
 	std::cout << " para real";
 }

/*
 *	prints the function declaration in the following format (using portuguese)
 *	Declaracão de funcão TYPE::Type: 
 *	+parametros:
 *	parametro <integer/real/boolean> param
 *	parametro arranjo <integer/real/boolean> de tamanho <size>: param
 *	Fim declaracao
 */
 void FunctionDeclaration::printTree(){
 	if(!isDef){
	 	std::cout << "Declaracão de funcão " << TYPE::femaleName[type] << ": ";
 	}
 	for( auto var = funcs.begin(); var != funcs.end(); var ++){
 		std::cout << dynamic_cast<Word *>(*var)->word;
 		if(next(var) != funcs.end())
 			std::cout << ", ";
 	}
 	std::cout << "\n+parametros:"<<endl;
 	for( auto var = params.begin(); var != params.end(); var ++){
 		// std::cout << "parametro ";
 		(*var)->isParam = true;
 		(*var)->printTree();
 		(*var)->isParam = false;
 		if(next(var) != params.end())
 			std::cout << "\n";
 	}

 	if(!isDef){
 		std::cout << "\nFim declaracao";
 	}
 }

 /*
 *	prints the function declaration in the following format (using portuguese)
 *	Declaracão de funcão TYPE::Type: 
 *	+parametros:
 *	parametro <integer/real/boolean> param
 *	parametro arranjo <integer/real/boolean> de tamanho <size>: param
 *	Fim declaracao
 */
 void FunctionDefinition::printTree(){
 	std::cout << "Definicão de funcão " << TYPE::femaleName[type] << ": ";
 	signature->printTree();

 	std::cout << "\n+corpo:"<<endl;
 	for( auto var = lines.begin(); var != lines.end(); var ++){
 		// std::cout << dynamic_cast<Word *>(*var)->word;
 		(*var)->printTree();
 		if(next(var) != lines.end())
 			std::cout << ", ";
 	}
 	std::cout << "\nFim definicão";
 }

/*
 *	prints the return value of the function declaration
 */
void FunctionReturn::printTree(){
 	std::cout << "Retorno de funcão: ";
 	value->printTree();
 	std::cout<<endl;
 }

/*
 *	prints each param in the list of params
 */
void Param::printTree(){
 	for( auto var = paramList.begin(); var != paramList.end(); var ++){
 		std::cout << "parametro ";
 		(*var)->isParam = true;
 		(*var)->printTree();
 		(*var)->isParam = false;
 		if(next(var) != paramList.end())
 			std::cout << "\n";
 	}
 }

/*
 *	prints the if block
 */
void IfBlock::printTree(){
	std::cout<<"Expressão condicional"<<endl;
	std::cout<<"+se: ";
	condition->printTree();
	std::cout<<endl;
	std::cout<<"+então: ";
	std::cout<<endl;
	for (Node* line: thenLines) {
		line->printTree();
	}
	if(hasElse){
		std::cout<<"+senão: ";
		std::cout<<endl;
		for (Node* line: elseLines) {
			line->printTree();
		}
	}
	std::cout<<"Fim expressão condicional";
}

/*
 *	prints the while block
 */
void WhileBlock::printTree(){
	std::cout<<"Laco"<<endl;
	std::cout<<"+enquanto: ";
	condition->printTree();
	std::cout<<endl;
	std::cout<<"+faca: ";
	std::cout<<endl;
	for (Node* line: lines) {
		line->printTree();
	}
	std::cout<<"Fim laco";
}

/*
 *	print the lines of the scope of the function definition
 * 	it's the same as the printing function of the block but without a break line in the end
 */
void FunctionBody::printTree(){
	for(Node* line : lines){
		line->printTree();
	}
}

/*
 *	prints the definition of a compound type, listing it's components
 */
void TypeDef::printTree(){
	std::cout<<"Definicao tipo: ";
	name->isComplex = true;
	name->printTree();
	name->isComplex = false;
	std::cout<<"\n+componentes:\n";
	for( auto var = nodes.begin(); var != nodes.end(); var ++){
 		(*var)->isParam = true;
 		(*var)->printTree();
 		(*var)->isParam = false;
 		if(next(var) != nodes.end())
 			std::cout << "\n";
 	}

	std::cout<<"\nFim definicao";
}

/*
 *	print the components of the scope of the compound type
 */
void TypeBody::printTree(){
	for( auto var = lines.begin(); var != lines.end(); var ++){
 		std::cout<<"Componente ";
 		(*var)->isParam = true;
 		(*var)->printTree();
 		(*var)->isParam = false;
 		if(next(var) != lines.end())
 			std::cout << "\n";
 	}
}
