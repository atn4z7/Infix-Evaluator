/*
Infix Evaluator Source 
Justin Karnes
CS 303 - Data Structures (Prof. Kuhail)
October 2014
*/
#pragma once
#include "Infix_Evaluator.h"
#include "String_Tokenizer.h"
#include <algorithm>

/* 
private:
std::vector<int> value_stack; // Vector based stack to hold non-operator tokens
std::vector<std::string> operator_stack; // Vector based stack to hold operator tokens
const static std::string OPERATOR_LIST; // List of operators             Index of operator in operator list
const static std::string OPERATOR_PRECEDENCE; // List of precedence.     will be index of precedence here.
char open_char; // This is the parenthetical operator that called the infix evaluator. If there is no paranthetical call
// then this value will be 'r'
int r_int; // Final result of the expression 
*/

// Static Variables
const std::string Infix_Evaluator::OPERATOR_LIST[] = { "==", "!=", "&&", "||", ">", ">=", "<", "<=","+", "-", "*", "/", "*", "^", "!" };
const int Infix_Evaluator::OPERATOR_PRECEDENCE[] = {1, 1, 2, 2, 3, 3, 3, 3, 4, 4, 5, 5, 5, 6, 7};
const int Infix_Evaluator::NUMBER_OF_OPERATORS = 15;

Infix_Evaluator::Infix_Evaluator(){} // Default constructor should do nothing

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
				if (tokenPrecedence >= stackPrecedence){
					// Push token to operator stack and return true
					operator_stack.push_back(operatorToken);
					return true;}
				else // Stack has a higher precedence than token
				{
					operator_stack.pop_back(); // Since we're about to use the operator from the stack, we pop it off.
					int RHS = PopValue(); // We'll need a value for the RHS to evaluate the token
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
	if (op == "!") return !RHS;
	else if (op == "++") return ++RHS;
	else if (op == "--") return --RHS;

	//These two lines will not execute if the operator was Uniary
	int LHS;
	LHS = PopValue(); 
	if (op == "^") return (int)pow((double)LHS, RHS);
	else if (op == "*") return (LHS*RHS);
	else if (op == "/") {
		if (RHS == 0) 
			throw std::logic_error("the denominator is 0");
		return (LHS / RHS);
	}
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
		theOperator = PopOperator();
		theValue = PopValue();
		PushToValueStack(operator_evaluator(theOperator, theValue));
	}
	theValue = PopValue();
	if (value_stack.empty()) return theValue;
	else throw std::logic_error("Oops");
}

//An, Hoang

Infix_Evaluator::Infix_Evaluator(std::string expression){
	/* This constructor will create two vectors, value_stack and operator_stack. It will then work on its internal logic
	to populate both stacks and evaluate the expression. When done, the Infix_Evaluator will also have some integer value
	r_int which represents the final value of the expression */
	string current;
	vector<string> tokens;
	tokens = Parser(expression);
	for (int i=0; i<tokens.size();i++){
		current = tokens[i];
		if (isNumber(current)){
			PushToValueStack(stoi(current));
		}
		else{
			PushToOperatorStack(current);
		}
	}
	int answer = evaluate();
	cout << "The answer " << answer << endl;
}

bool Infix_Evaluator::isNumber(string& s)
{
    string::iterator itr = s.begin();
    while (itr != s.end() && isdigit(*itr)) ++itr;
    return !s.empty() && itr == s.end();
}
vector<string> Infix_Evaluator::Parser(string& expression){
	vector<string> result;
	char token;
	string tempdigit = "";
	string tempoperator = "";
	//remove all spaces first
	expression.erase(remove(expression.begin(), expression.end(), ' '), expression.end());
	string::iterator itr = expression.begin();
	while (itr != expression.end()){
		if (isdigit(*itr)){ //is digit
			//for operator
			if (tempoperator != ""){
				result.push_back(tempoperator);
				tempoperator = "";
			}
			tempdigit+=*itr;
			++itr;
			if (itr == expression.end()){
				result.push_back(tempdigit);
				tempdigit = "";
			}
		}
		else{ //not digit
			//for digit
			if (tempdigit != ""){
				result.push_back(tempdigit);
				tempdigit = "";
			}

			char current = *itr;
			switch (current)
			{
			case '=':
				tempoperator+=current;
				if (tempoperator.length() == 2){ //full
					if ((tempoperator != ">=") && //check all valid of "*="
						(tempoperator != "<=") &&
						(tempoperator != "==") &&
						(tempoperator != "!=") ){
							result.push_back("Error Here ->");
					}
					result.push_back(tempoperator);
					tempoperator = "";
				}
				break;
			case '|':
				tempoperator+=current;
				if (tempoperator.length() == 2){ //full
					if (tempoperator != "||"){
						result.push_back("Error Here ->");
					}
					result.push_back(tempoperator);
					tempoperator = "";
				}
				break;
			case '&':
				tempoperator+=current;
				if (tempoperator.length() == 2){ //full
					if (tempoperator != "&&"){
						result.push_back("Error Here ->");
					}
					result.push_back(tempoperator);
					tempoperator = "";
				}
				break;
			case '+': // * + 3 will be pushed to vector fine (*,+,3)  even LHS is missing
				tempoperator+=current;
				++itr; //the 2nd char
				if (itr == expression.end() || *itr == '(' || *itr == '!'){ 
					//if + is at the end of the expression or the 2nd char is '(' or '!'
					result.push_back(tempoperator);
					tempoperator = "";
					--itr;
					break;
				}
				if (isdigit(*itr)) --itr; // only 1 +
				else{
					tempoperator+=*itr;
					if (tempoperator == "+-" || tempoperator == "++"){ // +- or ++
						++itr; //the 3rd char
						if (itr == expression.end() || *itr == '(' ){ //if + is at the end of the expression or the 3rd char is '('
							if (tempoperator.substr(0,1) != tempoperator.substr(1,1)){ //+-( case
								//add + then add -
								result.push_back(tempoperator.substr(0,1));
								result.push_back(tempoperator.substr(1,1));
								tempoperator = "";
							}
							else{//++( case
								result.push_back(tempoperator);
								tempoperator = "";
							}
							--itr;
							break;
						}
						if (isdigit(*itr)){
							if (tempoperator != "++") //+-number -> wrong format
								result.push_back("Error Here ->");
							result.push_back(tempoperator);
							itr--;
						}
						else{ //is operator
							tempoperator+=*itr;
							if (tempoperator == "+--"){
								result.push_back("+");
								result.push_back("--");
							}
							else if (tempoperator == "+++"){
								result.push_back("+");
								result.push_back("++");
							}
							else{
								result.push_back("Error Here ->");
								result.push_back(tempoperator);
							}
						}
					}
					else{
						result.push_back("Error Here ->");
						result.push_back(tempoperator);
					}
					tempoperator = "";
				}
				break;
			case '-':
				tempoperator+=current;
				++itr; //the 2nd char
				if (itr == expression.end() || *itr == '(' || *itr == '!' ){ 
					//if - is at the end of the expression or the 2nd char is '(' or '!'
					result.push_back(tempoperator);
					tempoperator = "";
					--itr;
					break;
				}
				if (isdigit(*itr)) --itr; // only 1 +
				else{
					tempoperator+=*itr;
					if (tempoperator == "-+" || tempoperator == "--"){ // -+ or --
						++itr; //the 3rd char
						if (itr == expression.end() || *itr == '('){ //if - is at the end of the expression or the 3rd char is '('
							if (tempoperator.substr(0,1) != tempoperator.substr(1,1)){  //-+( case
								//add - then add +
								result.push_back(tempoperator.substr(0,1));
								result.push_back(tempoperator.substr(1,1));
								tempoperator = "";
							}
							else{ //--( case
								result.push_back(tempoperator);
								tempoperator = "";
							}
							--itr;
							break;
						}
						if (isdigit(*itr)){
							if (tempoperator != "--") //+-number -> wrong format
								result.push_back("Error Here ->");
							result.push_back(tempoperator);
							itr--;
						}
						else{ //is operator
							tempoperator+=*itr;
							if (tempoperator == "-++"){
								result.push_back("-");
								result.push_back("++");
							}
							else if (tempoperator == "---"){
								result.push_back("-");
								result.push_back("--");
							}
							else{
								result.push_back("Error Here ->");
								result.push_back(tempoperator);
							}
						}
					}
					else{
						result.push_back("Error Here ->");
						result.push_back(tempoperator);
					}
					tempoperator = "";
				}
				break;
			case '/': 
			case '%':
			case '*': 
				tempoperator+=current;
				++itr;
				if (isdigit(*itr)) --itr;
				else{ // * followed by operator
					if (*itr == '!' || *itr == '+' || *itr == '-' || *itr == '('){ //* can be followed by !,+,- 
						result.push_back(tempoperator);
						--itr;
					}
					else{
						result.push_back("Error Here ->");
						result.push_back(tempoperator+*itr);
					}
					tempoperator = "";
				}
				break;
			case '>':
			case '<':
				tempoperator+=current;
				break;
			case '(':
			case ')':
				tempoperator+=current;
				result.push_back(tempoperator);
				tempoperator = "";
				break;
			case '!':
				if (tempoperator.length() >= 1){ //when we meet ! we first add anything that is still in tempoperator to vector
					result.push_back(tempoperator);
					tempoperator = "";
				}
				tempoperator+=current;
				++itr;
				if (isdigit(*itr)) --itr;
				else{ // ! followed by operator - * can be followed by !,+,-,= 
					if (*itr == '!' || *itr == '+' || *itr == '-'){
						result.push_back(tempoperator);
						--itr;
					}
					else if (*itr == '='){
						//add "!=" to vector
						result.push_back(tempoperator+*itr);
					}
					else{
						result.push_back("Error Here ->");
						result.push_back(tempoperator+*itr);
					}
					tempoperator = "";
				}
				break;
			case '^':
				tempoperator+=current;
				++itr;
				if (isdigit(*itr)) --itr;
				else{ // ^ followed by operator - ^ can be followed by !,+,-
					if (*itr == '!' || *itr == '+' || *itr == '-'){
						result.push_back(tempoperator);
						--itr;
					}
					else{
						result.push_back("Error Here ->");
						result.push_back(tempoperator+*itr);
					}
					tempoperator = "";
				}
				break;
				//default:	
			}
			++itr;
			if (itr == expression.end()){
				if (tempoperator != ""){
					result.push_back(tempoperator);
					tempoperator = "";
				}
			}
		}
	}
	return result;
}
