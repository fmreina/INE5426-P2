/*
 *	Staff
 *	references: 
 *	 - llpilla/compiler_examples/allen (github)
 */
#include "staff.h"

extern void yyerror(const char* s, ...);

using namespace TYPE;

/*
 *	returns the right type for the result of the binary operation
 */
Type TYPE::getBinType(Type left, OPERATION::Operation op, Type right){
	Type type;
	switch(op){
		// assign operation gets the type of the receiver
		case OPERATION::assign:
			type = left;
			// std::cout<<"left type "<<TYPE::maleName[left]<<std::endl;
			// std::cout<<"right type "<<TYPE::maleName[right]<<std::endl;
			if(left != right) ERROR::wrongTypeError(op, left, right);
			break;
		// numerical operations. If one is real the final type must be real
		case OPERATION::plus:
		case OPERATION::minus:
		case OPERATION::times:
		case OPERATION::divide:
			type = integer;
			if(left == real || right == real) type = real;
			if(left != integer && left != real) ERROR::wrongTypeError(op, integer, real, left);
			if(right != integer && right != real) ERROR::wrongTypeError(op, integer, real, right);
			break;
		// comparison operations must be boolean
		case OPERATION::greater:
		case OPERATION::greater_equals: 
		case OPERATION::smaller:
		case OPERATION::smaller_equals:
		case OPERATION::equals:
		case OPERATION::different:
			type = boolean;
			if(left != integer && left != real) ERROR::wrongTypeError(op, integer, real, left);
			if(right != integer && right != real) ERROR::wrongTypeError(op, integer, real, right);
			break;
		// AND/OR operations must be boolean
		case OPERATION::and_op:
		case OPERATION::or_op:
			type = boolean;
			if(left != integer && left != real) ERROR::wrongTypeError(op, boolean, left);
			if(right != integer && right != real) ERROR::wrongTypeError(op, boolean, right);
			break;
		default:
			type = unknown;
			break;
	}
	return type;
}

/*
 *	returns the right type for the result of the unary operation
 */
Type TYPE::getUnType(Type type, OPERATION::Operation op){
	Type final;
	switch(op){
		case OPERATION::u_minus:
			if(type != integer && type != real) ERROR::wrongTypeError(op, integer, real, type);
			final = type;
			break;
		case OPERATION::not_op:
			final = boolean;
			if(type != boolean) ERROR::wrongTypeError(op, boolean, type);
			break;
		default:
			final = type;
			break;
	}
	return final;
}

/*
 *	print error messages
 */
// void TYPE::wrongTypeError(OPERATION::Operation op, Type expected, Type given){
// 	yyerror("semântico: operacão %s espera %s mas recebeu %s.\n", OPERATION::name[op].c_str(), maleName[expected].c_str(), maleName[given].c_str());
// 	return;
// }

// void TYPE::wrongTypeError(OPERATION::Operation op, Type expected1, Type expected2, Type given){
// 	yyerror("semântico: operacão %s espera %s ou %s mas recebeu %s.\n", OPERATION::name[op].c_str(), maleName[expected1].c_str(), maleName[expected2].c_str(), maleName[given].c_str());
// 	return;
// }


void ERROR::wrongTypeError(OPERATION::Operation op, TYPE::Type expected, TYPE::Type given){
	yyerror("semântico: operacão %s espera %s mas recebeu %s.\n", OPERATION::name[op].c_str(), maleName[expected].c_str(), maleName[given].c_str());
	return;
}

void ERROR::wrongTypeError(OPERATION::Operation op, TYPE::Type expected1, TYPE::Type expected2, TYPE::Type given){
	yyerror("semântico: operacão %s espera %s ou %s mas recebeu %s.\n", OPERATION::name[op].c_str(), maleName[expected1].c_str(), maleName[expected2].c_str(), maleName[given].c_str());
	return;
}