//============================================================================
// Name        : Inflix_Evaluator.h
// Author      : Justin Karnes
// Description : C++ Infix Evaluator Class 
// CLass	   : CS 303 - Data Structures
// Date        : UMKC October 2014 
//============================================================================
   
   
   

#ifndef INFIX_EVALUATOR_H
#define INFIX_EVALUATOR_H

#include <stack>
#include <iostream>
#include <stdexcept>
#include "Token.h"
#include "Parser.h"
#include <math.h>
#include <algorithm>
#include "Syntax_Error.h"


class Infix_Evaluator{
	/* The Infix Evaluator class has two constructors, one of which is default. It accepts any
	mathematical expression in infix form and returns a value. If the expression is invalid
	then the class will throw errors appropriate to the invalid portion. Errors will be covered
	in depth at the operator evaluation function of this class. */
public:
	Infix_Evaluator();
	~Infix_Evaluator();
	bool PushToOperatorStack(Token opToken); 
	// See the Infix_Evaluator.cpp file for documentation on the functions which follow this line.
	bool PushToValueStack(int); 
	Token PopOperator(void);
	int PopValue(Token last_opToken);
	int operator_evaluator(Token opToken, int);
	std::string evaluate(void); 

	//An, Hoang
	//function that evaluates an inflix expression and returns the final answer as a string
	std::string evaluate(std::string& expression);
	//function that checks if a string is a number
	bool isNumber(std::string& token);
private:
	static const  std::string OPERATOR_LIST[]; // List of operators             Index of operator in operator list
	static const int OPERATOR_PRECEDENCE[]; // List of precedence.             will be index of precedence here.
	static const int NUMBER_OF_OPERATORS;
	std::stack<int> value_stack; // a stack to hold non-operator tokens
	std::stack<Token> operator_stack; // a stack to hold operator tokens
};



#endif
