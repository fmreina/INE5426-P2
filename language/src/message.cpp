#include "staff.h"
#include "message.h"
#include "ast.h"
#include <iostream>
#include <vector>

extern void yyerror(const char* s, ...);

using namespace std;
using namespace TYPE;
	
	void MESSAGES::wrongTypeError(OPERATION::Operation op, TYPE::Type expected, TYPE::Type given){
		yyerror("\n - semântico: operacão %s espera %s mas recebeu %s.\n", OPERATION::name[op].c_str(), TYPE::maleName[expected].c_str(), TYPE::maleName[given].c_str());
	}

	void MESSAGES::wrongTypeError(OPERATION::Operation op, TYPE::Type expected1, TYPE::Type expected2, TYPE::Type given){
		yyerror("\n - semântico: operacão %s espera %s ou %s mas recebeu %s.\n", OPERATION::name[op].c_str(), TYPE::maleName[expected1].c_str(), TYPE::maleName[expected2].c_str(), TYPE::maleName[given].c_str());
	}

	void MESSAGES::assignValueMessage(AST::Node* left, AST::Node* right){
		std::cout<<  " Ação: Foi atribuido o ";
		right->printTree();
		std::cout<<  " à ";
		left->printTree(); 
		std::cout<<"."<<std::endl;
	}

	void MESSAGES::assignIntegerPartMessage(AST::Node* left, AST::Node* right){
		std::cout<<  " Ação: Foi atribuido a parte inteira de ";
		right->printTree();
		std::cout<<  " à ";
		left->printTree(); 
		std::cout<<"."<<std::endl;
	}

	void MESSAGES::indexOutOfBounds(int position, int lengh){
		yyerror("\n - Posicão válida. tentou acessar %d, mas o tamanho do array é %d.\n", position, lengh);
	}