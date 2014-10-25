/* 
   C++ Infix Evaluator Class 
   Justin Karnes
   CS 303 - Data Structures
   UMKC October 2014 
*/

#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
#include <math.h>

class Infix_Evaluator{
	/* The Infix Evaluator class has two constructors, one of which is default. It accepts any
	mathematical expression in infix form and returns a value. If the expression is invalid
	then the class will throw errors appropriate to the invalid portion. Errors will be covered
	in depth at the operator evaluation function of this class. */
public:
	Infix_Evaluator();
	Infix_Evaluator(std::string);
	~Infix_Evaluator();

	bool PushToOperatorStack(std::string); 
	bool PushToValueStack(int); 
	std::string PopOperator(void);
	int PopValue(void);
	int operator_evaluator(std::string, int);
	static int NUMBER_OF_OPERATORS;

	struct Token
	{
		std::string the_data;
		char TokenType; // 'D' for number, 'U' for Uniary, 'B' for Binary
		Token(std::string, char);
	};
	Infix_Evaluator::Token ParseNewToken(std::string::iterator&, std::string::iterator&, int);
	std::vector<Infix_Evaluator::Token> ParseExpression(std::string& expression);
	int r_int; // Final result of the expression

private:
	std::vector<int> value_stack; // Vector based stack to hold non-operator tokens
	std::vector<std::string> operator_stack; // Vector based stack to hold operator tokens
	static std::string OPERATOR_LIST[]; // List of operators             Index of operator in operator list
	static int OPERATOR_PRECEDENCE[]; // List of precedence.             will be index of precedence here.
	int isBinary(std::string&);
	int evaluate(void); // Evaluates the expression once it is fully parsed.
};
