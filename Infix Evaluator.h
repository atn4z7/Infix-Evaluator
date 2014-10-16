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
	/* One of the more complicated functions. PushToOperatorStack follows the following algorithm:
	A) Check if the operator is valid:
		1) if true:
			a) While HaveNotGeneratedReturn:
				i) Check to see if the operator stack is empty:
					1) if true:
						A) push operatorToken to operator_stack
						B) return true
					2) if false:
						A) check precedence of top operator on stack
							I) If operatorToken's precedence >= operator_stack.top()'s precedence:
								1) Push operatorToken onto operator_stack
								2) return true
							II) Else:
								1) EvalToken = operator_stack.top()
								2) operator_stack.pop()
								3) RHS = value_stack.top()
								4) value_stack.pop()
								5) Call operator_evaluator(EvalToken, RHS)
		2) if false:
			a) throw std::SyntaxError("Invalid Expression!")
	*/

	// See the Infix_Evaluator.cpp file for documentation on the functions which follow this line.
	bool PushToValueStack(int); 
	std::string PopOperator(void);
	int PopValue(void);
	int operator_evaluator(std::string, int);
	// Temporarilly public for testing
	static std::string OPERATOR_LIST[]; // List of operators             Index of operator in operator list
	static int OPERATOR_PRECEDENCE[]; // List of precedence.             will be index of precedence here.
	static int NUMBER_OF_OPERATORS;
	int evaluate(void); // Function for Testing. DO NOT USE IN LIVE

private:
	std::vector<int> value_stack; // Vector based stack to hold non-operator tokens
	std::vector<std::string> operator_stack; // Vector based stack to hold operator tokens
	/*static std::string OPERATOR_LIST[]; // List of operators             Index of operator in operator list
	static int OPERATOR_PRECEDENCE[]; // List of precedence.             will be index of precedence here.
	static int NUMBER_OF_OPERATORS;*/
	char open_char; // This is the parenthetical operator that called the infix evaluator. If there is no paranthetical call
	                // then this value will be 'r'
	int r_int; // Final result of the expression
};
