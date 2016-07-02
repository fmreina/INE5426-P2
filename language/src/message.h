 #pragma once

#include "ast.h"
#include "staff.h"
#include <iostream>
#include <vector>

using namespace std;

extern void yyerrer(const char *s, ...);

namespace MESSAGES{
	/*
	 *	print error messages
	 */
	void wrongTypeError(OPERATION::Operation op, TYPE::Type expected, TYPE::Type guiven);

	void wrongTypeError(OPERATION::Operation op, TYPE::Type expected1, TYPE::Type expected2, TYPE::Type guiven);

	void assignValueMessage(AST::Node* left, AST::Node* right);

	void assignIntegerPartMessage(AST::Node* left, AST::Node* right);

}