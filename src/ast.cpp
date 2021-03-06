/*
 *	Abstract Syntax Tree (AST)
 *	references: 
 *	 - llpilla/compiler_examples/allen (github)
 *	 - Aho, Alfred, et al - Compilers: principles, techniques, and tools - 2nd ed. 2007 Pearson Education
 *	 - Levine, John - Flex & Bison - Unix text processing tools - 2009 O'Reilly Media
 */

#include "ast.h"
// #include "message.h"
#include "symbolTable.h"

using namespace AST;
// using namespace MESSAGES;

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
				std::cout << "Atribuicão de valor para arranjo "<<  TYPE::maleName[left->type] << " ";
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

void BinOp::math(Node *newLeft, OPERATION::Operation op, Node *newRight){
	// if(newLeft->type == TYPE::boolean) MESSAGES::wrongTypeError(op, TYPE::integer, TYPE::real, newLeft->type);
	// if(newRight->type == TYPE::boolean) MESSAGES::wrongTypeError(op, TYPE::integer, TYPE::real, newRight->type);
	if(newLeft->type == TYPE::boolean) MESS::wrongTypeError(op, TYPE::integer, TYPE::real, newLeft->type);
	if(newRight->type == TYPE::boolean) MESS::wrongTypeError(op, TYPE::integer, TYPE::real, newRight->type);
	this->type = TYPE::integer;
	if(newLeft->type == TYPE::real || newRight->type == TYPE::real){
		this->type = TYPE::real;
	}
		this->left = newLeft;
		this->op = op;
		this->right = newRight;
}

void BinOp::comparison(Node *newLeft, OPERATION::Operation op, Node *newRight){
	// if(newLeft->type != TYPE::integer && newLeft->type != TYPE::real) MESSAGES::wrongTypeError(op, TYPE::integer, TYPE::real, newLeft->type);
	// if(newRight->type != TYPE::integer && newRight->type != TYPE::real) MESSAGES::wrongTypeError(op, TYPE::integer, TYPE::real, newRight->type);
	if(newLeft->type != TYPE::integer && newLeft->type != TYPE::real) MESS::wrongTypeError(op, TYPE::integer, TYPE::real, newLeft->type);
	if(newRight->type != TYPE::integer && newRight->type != TYPE::real) MESS::wrongTypeError(op, TYPE::integer, TYPE::real, newRight->type);
	this->op = op;
	this->left = newLeft;
	this->right = newRight;
	this->type = TYPE::boolean;
}

void BinOp::unOperation(Node *newLeft, OPERATION::Operation op, Node *newRight){
	// if(newLeft->type != TYPE::integer && newLeft->type != TYPE::real) MESSAGES::wrongTypeError(op, TYPE::boolean, newLeft->type);
	// if(newRight->type != TYPE::integer && newRight->type != TYPE::real) MESSAGES::wrongTypeError(op, TYPE::boolean, newRight->type);
	if(newLeft->type != TYPE::integer && newLeft->type != TYPE::real) MESS::wrongTypeError(op, TYPE::boolean, newLeft->type);
	if(newRight->type != TYPE::integer && newRight->type != TYPE::real) MESS::wrongTypeError(op, TYPE::boolean, newRight->type);
	this->op = op;
	this->left = newLeft;
	this->right = newRight;
	this->type = TYPE::boolean;
}

void BinOp::assign(Node *newLeft, OPERATION::Operation op, Node *newRight){
	/*
	 * if left type is the same as right type, ok!
	 */
	 auto arr = dynamic_cast<AST::Word *>(newLeft);
	if(arr->kind == KIND::array){
		auto position = dynamic_cast<Value*>(arr->size);
		if(position){
			int pos = std::stoi(position->value);
			int lengh = std::stoi(arr->lengh);
			if(pos <= lengh-1){
				BinOp::validateAndAssign(newLeft, op, newRight);
			} else {
				//FIXME: how to avoid segmentation fault if do not set the values?
				//index out of bounds
				// MESSAGES::indexOutOfBounds(pos, lengh-1);
				MESS::indexOutOfBounds(pos, lengh-1);
				this->left = newLeft;
				this->op = op;
				this->right = newRight;
			}
		} else {
			// TODO: send the value when doing SymbolTable::assignVariable and get it back here to set as index of the array
			// auto word = dynamic_cast<Word*>(arr->size);
			// auto symbol = symTab.entryList[word->word];
			// cout<<"symbol:value = "<< word->word << ":" << symbol.value;
			//FIXME: it does not control if the index is in the bounds of the array
			BinOp::validateAndAssign(newLeft, op, newRight);
		}
	}
	else{
		BinOp::validateAndAssign(newLeft, op, newRight);
	}
}

void BinOp::validateAndAssign(Node *newLeft, OPERATION::Operation op, Node *newRight){
	if ( newLeft->type == newRight->type ){
		this->left = newLeft;
		this->op = op;
		this->right = newRight;
	} 
	else {
		/*
		 * if left type is different from right type, send an error and take an action
		 */
		// MESSAGES::wrongTypeError(op, newLeft->type, newRight->type);
		MESS::wrongTypeError(op, newLeft->type, newRight->type);
		this->left = newLeft;
		this->op = op;
		if(newLeft->type == TYPE::integer){ 
			coerceToInteger(newLeft, newRight);
		} else if (newLeft->type == TYPE::real){
			// std::cout<< " FIXME: assigning real 0.0" << std::endl; // what to do in this case?
			this->right = new Value("0.0", TYPE::real);
			// MESSAGES::assignValueMessage(this->left, this->right);
			MESS::assignValueMessage(this->left, this->right);
		} else if (newLeft->type == TYPE::boolean){
			// std::cout<< " FIXME: assigning boolean false" << std::endl; // what to do in this case?
			this->right = new Value("false", TYPE::boolean);
			// MESSAGES::assignValueMessage(this->left, this->right);
			MESS::assignValueMessage(this->left, this->right);
		}
	}
}

void BinOp::coerceToInteger(Node *newLeft, Node *newRight){
	auto val = dynamic_cast<AST::Value *>(newRight);
	if(val){
		if(newRight->type == TYPE::real){ 
			/* from real to int */
			auto cropPosition = val->value.find(".");
			auto intValue = val->value.substr(0, cropPosition);
			val->value = intValue;
			val->type = TYPE::integer;

			this->right = val;
		}else { 
			/* from bool to int */
			// std::cout<< " FIXME: assigning integer 0" << std::endl; // what to do in this case?
			this->right = new Value("0", TYPE::integer);
		}
		// MESSAGES::assignValueMessage(this->left, this->right);
		MESS::assignValueMessage(this->left, this->right);
	}else{
		// in case it can't cast
		// std::cout<< "Casting Error: at AST::BinOP at ast.h" << std::endl;
		this->right = newRight;
		// MESSAGES::assignIntegerPartMessage(this->left, this->right);
		MESS::assignIntegerPartMessage(this->left, this->right);
	}
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

void UnOp::checkType(TYPE::Type type, OPERATION::Operation op){
	switch(op){
		case OPERATION::u_minus:
			if(type != TYPE::integer && type != TYPE::real){ 
				// MESSAGES::wrongTypeError(op, TYPE::integer, TYPE::real, type);}
				MESS::wrongTypeError(op, TYPE::integer, TYPE::real, type);}
			else {
				this->node->type = type;}
			break;
		case OPERATION::not_op:
			// if(type != TYPE::boolean) MESSAGES::wrongTypeError(op, TYPE::boolean, type);
			if(type != TYPE::boolean) MESS::wrongTypeError(op, TYPE::boolean, type);
			break;
		default:
			break;
	}				
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
void AST::Value::printTree(){
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
Node* Node::coerce(Node* left){
 	if(this->needCoersion(this->type, left->type)){
 		this->type = TYPE::real;
 		return new AST::Coercion(this);
 	}

 	// this->type = TYPE::integer; // do not make coercion, keep the types as it is
 	return this;
}

/*
 *	check if needs to make a coercion. If this->node is integer and the other is real return true.
 */
bool Node::needCoersion(TYPE::Type right, TYPE::Type left){
 	return(right == TYPE::integer && left == TYPE::real);
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
 	for( auto var = funcs.begin(); var != funcs.end(); var ++){
 		std::cout << dynamic_cast<Word *>(*var)->word;
 		if(next(var) != funcs.end())
 			std::cout << ", ";
 	}
 	std::cout << "\n+parametros:"<<endl;
 	for( auto var = params.begin(); var != params.end(); var ++){
 		(*var)->isParam = true;
 		(*var)->printTree();
 		(*var)->isParam = false;
 		if(next(var) != params.end())
 			std::cout << "\n";
 	}
 	//

 	std::cout << "\n+corpo:"<<endl;
 	for( auto var = lines.begin(); var != lines.end(); var ++){
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
 *	prints the definition of a for loop
 */
void FromTil_Block::printTree(){
	if(error){
		std::cout<< "Ocorreu erro na declaracão e a estrutura \"de <inteiro> ate <inteiro>\" não foi computada.";
	}else {
		std::cout<<"Laco"<<endl;
		std::cout<<"+de ";
		fromExpr->printTree();
		std::cout<<" até ";
		tilExpr->printTree();
		std::cout<<endl;
		std::cout<<"+faca: ";
		std::cout<<endl;
		for (Node* line: lines) {
			line->printTree();
		}
		std::cout<<"Fim laco";
	}
}
