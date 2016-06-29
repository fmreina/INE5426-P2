/*
 *	Staff
 *	references: 
 *	 - llpilla/compiler_examples/allen (github)
 *	 - Aho, Alfred, et al - Compilers: principles, techniques, and tools - 2nd ed. 2007 Pearson Education
 *	 - Levine, John - Flex & Bison - Unix text processing tools - 2009 O'Reilly Media
 */
 #pragma once

#include <iostream>
#include <vector>

using namespace std;

extern void yyerrer(const char *s, ...);

namespace OPERATION{

	/*
	 *	enumeration for the possible kinds of operation
	 */
	enum Operation { 
		plus, 
		minus, 
		times, 
		divide, 
		assign, 
		greater, 
		greater_equals, 
		smaller, 
		smaller_equals, 
		equals, 
		different, 
		not_op, 
		and_op, 
		or_op,
		u_minus,
		parenthesis
	};

	/*
	 *	Mapping for printing messages
	 */
	const std::vector<std::string> name = {
        "soma",
        "subtracao",
        "multiplicacao",
        "divisao",
        "atribuicao",
        "maior",
        "maior ou igual",
        "menor",
        "menor ou igual",
        "igual",
        "diferente",
        "nao",
        "e",
        "ou",
        "menos unario",
        "parenteses"
    };

    /*
	 *	Mapping for checking gender
	 */
    const std::vector<bool> maleGender = {
        false,  //soma 
        false,  //subtracao 
        false,  //multiplicacao 
        false,  //divisao 
        false,  //atribuicao (nunca usado)
        true,   //maior 
        true,   //maior ou igual 
        true,   //menor 
        true,   //menor ou igual 
        true,   //igual 
        true,   //diferente 
        true,   //nao 
        true,   //e 
        true,   //ou 
        true,   //menos unario 
        false   //parenteses (nunca usado)
    };
}


namespace TYPE {

	/*
	 *	ebuneration for the possible types of varialbe
	 */
	enum Type { 
		integer, 
		real, 
		boolean,
		unknown
	};

	/*
	 *	Mapping for printing messages
	 */
	const std::vector<std::string> femaleName = {
		"inteira",
		"real",
		"booleana",
		"unknown"
	};

	/*
	 *	Mapping for printing messages
	 */
    const std::vector<std::string> maleName = {
    	"inteiro",
    	"real",
    	"booleano"
    };

	static Type lastType;

	/*
	 *	returns the right type for the result of the binary operation
	 */
	Type getBinType(Type left, OPERATION::Operation op, Type right);

	/*
	 *	returns the right type for the result of the unary operation
	 */
	Type getUnType(Type type, OPERATION::Operation op);

	/*
	 *	print error messages
	 */
	// void wrongTypeError(OPERATION::Operation op, Type expected, Type guiven);
	// void wrongTypeError(OPERATION::Operation op, Type expected1, Type expected2, Type guiven);
}

namespace ERROR{

	void wrongTypeError(OPERATION::Operation op, TYPE::Type expected, TYPE::Type guiven);

	void wrongTypeError(OPERATION::Operation op, TYPE::Type expected1, TYPE::Type expected2, TYPE::Type guiven);
}