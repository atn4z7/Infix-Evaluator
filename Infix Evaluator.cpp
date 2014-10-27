/*
Infix Evaluator Source 
Justin Karnes
CS 303 - Data Structures (Prof. Kuhail)
October 2014
*/
#pragma once
#include "Infix Evaluator.h"

//Structure

Infix_Evaluator::Token::Token(std::string newData, char newType)
{
	the_data = newData; TokenType = newType;
}

// Static Variables
std::string Infix_Evaluator::OPERATOR_LIST[] { "==", "!=", "&&", "||", ">", ">=", "<", "<=",
"+", "-", "*", "/", "^", "!", "++", "--", "(", ")" }; // Binary Operators are 0-12
int Infix_Evaluator::OPERATOR_PRECEDENCE[] {1, 1, 2, 2, 3, 3, 3, 3, 4, 4, 5, 5, 6, 7, 7, 7, 0, 8};
int Infix_Evaluator::NUMBER_OF_OPERATORS = 18;

Infix_Evaluator::Infix_Evaluator(){} // Default constructor should do nothing

Infix_Evaluator::Infix_Evaluator(std::string expression){
	std::vector<Infix_Evaluator::Token> expressionVector;
	expressionVector = ParseExpression(expression);
	for (std::vector<Infix_Evaluator::Token>::iterator Q = expressionVector.begin();
		Q != expressionVector.end(); Q++){
		std::cout << "The token is: " << Q->TokenType << " with a value of: " << Q->the_data << std::endl;
		if (Q->TokenType == 'D' && Q->the_data!=")")
			PushToValueStack(stoi(Q->the_data));
		else PushToOperatorStack(Q->the_data);
	}
	r_int = evaluate();
}

Infix_Evaluator::~Infix_Evaluator(){
/* The end of the construtor should end with the class having either primative data types or empty vectors
   However, in the event of an error, the destructor needs to make sure the vectors are empty, and if not
   delete them. Since we are using std::vector, std::vector::~vector() should handle this automatically. Test it.*/ }

bool Infix_Evaluator::PushToValueStack(int newValue){
	/* This function accepts an integer parameter, which can come from a token, or from an evaluation, and puts it
	into the value_stack vector. Always pushes back to the end of the vector. std::vector::push_back() handles resizing.*/
	try
	{
		value_stack.push_back(newValue);
		return true;
	}
	// The following line will catch any exception which occurs in attempting to push the value to the vector
	catch (...) { 
		std::cout << "An error occured in attempting to push " << newValue << " onto the value_stack!" << std::endl;
		return false; }
}

bool Infix_Evaluator::PushToOperatorStack(std::string operatorToken)
{
	int tokenIndex = -1, stackIndex = -1;
	int stackPrecedence, tokenPrecedence;
	int RHS;
	std::string stackOperator;
	// Check if the operator is valid:
	for (int i = 0; i < NUMBER_OF_OPERATORS; i++)
	{
		if (operatorToken == OPERATOR_LIST[i]){
			tokenIndex = i; break;
		}} 
	if (tokenIndex != -1) // If operator exists, then index >= 0
	{
		while (true) // Do the following until a return condition is met
		{
			if (operator_stack.empty()) // Operator Stack is empty
			{
				operator_stack.push_back(operatorToken);
				return true;
			}
			else // operator_stack was not empty
			{
				// Find precedence of the top of the operator stack
				stackOperator = operator_stack.back();
				for (int j = 0; j < NUMBER_OF_OPERATORS; j++)
				{
					if (stackOperator == OPERATOR_LIST[j]){
						stackIndex = j; break;
					}}
				stackPrecedence = OPERATOR_PRECEDENCE[stackIndex];
				tokenPrecedence = OPERATOR_PRECEDENCE[tokenIndex];
				
				// Compare the precdences of the stack and the token
				
				if (operatorToken == ")") // We've hit a closing parenthesis
				{
					if (operator_stack.empty())
						throw std::logic_error("Closing Parenthesis without Open Parenthesis");
					while (operator_stack.back() != "(")
					{
						if (!operator_stack.empty()){
							RHS = PopValue();
							if (PushToValueStack(operator_evaluator(stackOperator, RHS))){
								std::cout << "Evaluated an operator." << std::endl;
								operator_stack.pop_back();
								stackOperator = operator_stack.back();
							}
						}
						else throw std::logic_error("Unbalanced Parenthesis.");
					}
					operator_stack.pop_back();
					return true;
				}
				else if (tokenPrecedence > stackPrecedence || operatorToken == "("){
					// Push token to operator stack and return true
					operator_stack.push_back(operatorToken);
					return true;
				}
				else // Stack has a higher precedence than token
				{
					operator_stack.pop_back(); // Since we're about to use the operator from the stack, we pop it off.
					RHS = PopValue(); // We'll need a value for the RHS to evaluate the token
					if (PushToValueStack(operator_evaluator(stackOperator, RHS))){
						std::cout << "Evaluated an operator." << std::endl;
					}
					else
					{
						throw std::logic_error("Something went wrong while pushing a value onto the ValueStack");
					}
				}
			}
		}
	}
	else throw std::logic_error("Invalid Expression!");
}

std::string Infix_Evaluator::PopOperator(void){
	// This function simultaneously removes the top element of the operator stack and returns it
	// This function throws a length error if the stack was empty when this function is called
	if (operator_stack.empty()) throw std::length_error("Operator Stack was empty");
	std::string returnValue = operator_stack.back();
	operator_stack.pop_back();
	return returnValue;
}

int Infix_Evaluator::PopValue(void){
	/* This function, like the PopOperator function simultaneously returns and pops the top value
	of value_stack. This function throws length_error if the stack was empty when this function is called. */
	if (value_stack.empty()) throw std::length_error("Value Stack was empty");
	int returnValue = value_stack.back();
	value_stack.pop_back();
	return returnValue;
}

int Infix_Evaluator::operator_evaluator(std::string op, int RHS){
	/* This function is the method by which the infix evaluator figures out what value to push onto the value
	   stack. None of us know how to program a stream which evaluates an expression, which might be the fastest
	   way of doing this, so we've taken the following option below:
	*/
	
	//Uniary operators. These do not require a LHS value, we have not taken one from valueStack until after this point.
	std::cout << "PRECONDITION! Operator is " << op << " and RHS is " << RHS << std::endl;
	for (std::vector<int>::iterator Q = value_stack.begin(); Q != value_stack.end(); Q++){ std::cout << *Q << " "; }
	std::cout << std::endl;
	std::cout << "Operator Stack: ";
	for (std::vector<std::string>::iterator Q = operator_stack.begin(); Q != operator_stack.end(); Q++){ std::cout << *Q << " "; }
	std::cout << std::endl;
	
	if (op == "!") return !RHS;
	else if (op == "++") return ++RHS;
	else if (op == "--") return --RHS;
	
	//These two lines will not execute if the operator was Uniary
	int LHS;
	LHS = PopValue(); 
	if (op == "^") return (int)pow((double)LHS, RHS);
	else if (op == "*") return (LHS*RHS);
	else if (op == "/") return (LHS / RHS);
	else if (op == "%") return (LHS % RHS);
	else if (op == "+") return (LHS + RHS);
	else if (op == "-") return (LHS - RHS);
	else if (op == ">") return (LHS > RHS);
	else if (op == ">=") return (LHS >= RHS);
	else if (op == "<") return (LHS < RHS);
	else if (op == "<=") return (LHS <= RHS);
	else if (op == "&&") return (LHS && RHS);
	else if (op == "||") return (LHS || RHS);
	else if (op == "==") return (LHS == RHS);
	else if (op == "!=") return (LHS != RHS);
	else throw std::logic_error("Somehow, an unrecognized operator made it onto the stack!");
}

int Infix_Evaluator::evaluate(void){
	std::string theOperator;
	int theValue;
	while (!operator_stack.empty())
	{
		std::cout << "PRECONDITION" << std::endl;
		std::cout << "Value Stack: ";
		for (std::vector<int>::iterator Q = value_stack.begin(); Q != value_stack.end(); Q++){ std::cout << *Q << " "; }
		std::cout << std::endl;
		std::cout << "Operator Stack: ";
		for (std::vector<std::string>::iterator Q = operator_stack.begin(); Q != operator_stack.end(); Q++){ std::cout << *Q << " "; }
		std::cout << std::endl;
		theOperator = PopOperator();
		theValue = PopValue();
		PushToValueStack(operator_evaluator(theOperator, theValue));
	}
	theValue = PopValue();
	if (value_stack.empty()) return theValue;
	else throw std::logic_error("Oops");
}

// Parsing Tokens
Infix_Evaluator::Token Infix_Evaluator::ParseNewToken(std::string::iterator& Q,
	std::string::iterator& E, int mode){

	std::cout << "Entered ParseNewToken function" << std::endl;
	std::string temp_string = "";
	if (mode == 0)  // Uniary or Numeric Token expected
	{
		std::cout << "Mode 0" << std::endl;
		std::cout << "*Q is: " << *Q << std::endl;
		if (*Q == '-') // Negative Numeric Token or Prefix --
		{
			std::cout << "Negative Integer or Prefix -- Incomming!" << std::endl;
			temp_string += *Q;
			Q++;
			if (*Q == '-') // Prefix --
			{
				std::cout << "Prefix Decrement Detected!" << std::endl;
				temp_string += *Q;
				Q++;
				std::cout << "Returning a Token with value " << temp_string << std::endl;
				return Infix_Evaluator::Token(temp_string, 'U');
			}
			while (isdigit(*Q)) // Negative number. Only gets to this point if not --
			{
				std::cout << "In Mode 0 Numeric While" << std::endl;
				std::cout << "*Q is: " << *Q << std::endl;
				temp_string += *Q;
				++Q;
				while (Q != E && *Q == ' ')
				{
					Q++;
				}
				if (Q == E) break;
			}
			std::cout << "Returning a Token with value " << temp_string << std::endl;
			return Infix_Evaluator::Token(temp_string, 'D'); // ONLY RUNS IF LOOP TERMINATES
		}
		else if (isdigit(*Q))// *Q was a digit but won't be negative.
		{
			while (isdigit(*Q))
			{
				std::cout << "In Mode 0 Numeric While" << std::endl;
				std::cout << "*Q is: " << *Q << std::endl;
				temp_string += *Q;
				++Q;
				while (Q!=E && *Q == ' ')
				{
					Q++;
				}
				if (Q == E) break;
			}
			std::cout << "Returning a Token with value " << temp_string << std::endl;
			return Infix_Evaluator::Token(temp_string, 'D'); // ONLY RUNS IF LOOP TERMINATES
		}
		while (!isdigit(*Q)) // Q will be an operator token
		{
			while (Q != E && *Q == ' ')
			{
				Q++;
			}
			std::cout << "In Mode 0 Operator While" << std::endl;
			if (temp_string.length() == 2 || temp_string == "(" || temp_string == ")" || temp_string == "!"){
				std::cout << "Returning a Token with value " << temp_string << std::endl;
				return Infix_Evaluator::Token(temp_string, 'U');
			}
			else
			{
				std::cout << "*Q is: " << *Q << std::endl;
				temp_string += *Q;
				Q++;
			}
		}
		std::cout << "Returning a Token with value " << temp_string << std::endl;
		return Infix_Evaluator::Token(temp_string, 'U');
		}

		if (mode == 1) // Binary Token Expected
		while (!isdigit(*Q)) // Q will be an operator token
		{
			while (Q!=E && *Q == ' '){
				Q++;
			}
			std::cout << "In Mode 1 While" << std::endl;
			// Include in the following line the condition:
			// IF temp_string IN Binary_Operator_Array
			if (temp_string.length() == 2 || ((isBinary(temp_string) != -1) && *Q != '='))
			{
				/*if (temp_string == ")")
				{
					std::cout << "Returning a Token with value " << temp_string << std::endl;
					return Infix_Evaluator::Token(temp_string, 'D');
				}*/
				std::cout << "Returning a Token with value " << temp_string << std::endl;
				return Infix_Evaluator::Token(temp_string, 'B');
			}
			else if (temp_string == ")")
			{
				std::cout << "Returning a Token with value " << temp_string << std::endl;
				return Infix_Evaluator::Token(temp_string, 'D');
			}
			else
			{
				std::cout << "*Q is: " << *Q << std::endl;
				temp_string += *Q;
				Q++;
				if (Q == E) break;
			}
		}
		std::cout << "Returning a Token with value " << temp_string << std::endl;
		return Infix_Evaluator::Token(temp_string, 'B');
	}

// Parse a String into Tokens
std::vector<Infix_Evaluator::Token> Infix_Evaluator::ParseExpression(std::string& expression)
{
	std::vector<Infix_Evaluator::Token> FinalVector;
	std::string temp_string = "";
	std::string::iterator Q = expression.begin();
	std::string::iterator E = expression.end();
	char lastTokenType;
	// I assume that the first character of the expression will either be
	// Uniary or Integer
	Infix_Evaluator::Token newToken = ParseNewToken(Q, E, 0);
	FinalVector.push_back(newToken);

	while (Q != expression.end())
	{
		lastTokenType = FinalVector.back().TokenType;
		if (lastTokenType == 'U' || lastTokenType == 'B') // Last token was Uniary or Binary operator
		{
			// Next Acceptable token SHOULD BE uniary or numeric
			newToken = ParseNewToken(Q, E, 0);
			if (newToken.TokenType == 'U' || newToken.TokenType == 'D')
				FinalVector.push_back(newToken);
			else
				throw std::logic_error("Unacceptable Expression");

		}
		else // Last token was an integer
		{
			newToken = ParseNewToken(Q, E, 1);
			if (newToken.TokenType == 'B' || newToken.the_data == ")")
			{
				FinalVector.push_back(newToken);
			}
			else
				throw std::logic_error("Unacceptable Expression");
		}
	}
	return FinalVector;
}

// Find out if an operator is a binary operator
// Runs at worst case, O(1)
int Infix_Evaluator::isBinary(std::string& test_string)
{
	if (test_string == ")") return 8;
	for (int i = 0; i < 13; i++)
	{
		if (OPERATOR_LIST[i] == test_string) return i;
	}
	
	return -1;
}