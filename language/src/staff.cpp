/*
 *	Staff
 *	references: 
 *	 - llpilla/compiler_examples/allen (github)
 */
#include "staff.h"
#include "message.h"

extern void yyerror(const char* s, ...);

using namespace TYPE;
using namespace MESSAGES;

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
			if(left != right) MESSAGES::wrongTypeError(op, left, right);
			break;
		// numerical operations. If one is real the final type must be real
		case OPERATION::plus:
		case OPERATION::minus:
		case OPERATION::times:
		case OPERATION::divide:
			type = integer;
			if(left == real || right == real) type = real;
			if(left != integer && left != real) MESSAGES::wrongTypeError(op, integer, real, left);
			if(right != integer && right != real) MESSAGES::wrongTypeError(op, integer, real, right);
			break;
		// comparison operations must be boolean
		case OPERATION::greater:
		case OPERATION::greater_equals: 
		case OPERATION::smaller:
		case OPERATION::smaller_equals:
		case OPERATION::equals:
		case OPERATION::different:
			type = boolean;
			if(left != integer && left != real) MESSAGES::wrongTypeError(op, integer, real, left);
			if(right != integer && right != real) MESSAGES::wrongTypeError(op, integer, real, right);
			break;
		// AND/OR operations must be boolean
		case OPERATION::and_op:
		case OPERATION::or_op:
			type = boolean;
			if(left != integer && left != real) MESSAGES::wrongTypeError(op, boolean, left);
			if(right != integer && right != real) MESSAGES::wrongTypeError(op, boolean, right);
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
			if(type != integer && type != real) MESSAGES::wrongTypeError(op, integer, real, type);
			final = type;
			break;
		case OPERATION::not_op:
			final = boolean;
			if(type != boolean) MESSAGES::wrongTypeError(op, boolean, type);
			break;
		default:
			final = type;
			break;
	}
	return final;
}
