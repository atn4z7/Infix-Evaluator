/*
Infix Evaluator Source 
Justin Karnes
CS 303 - Data Structures (Prof. Kuhail)
October 2014
*/

#include "Infix_Evaluator.h"

// Static Variables
const std::string Infix_Evaluator::OPERATOR_LIST[] ={ "==", "!=", "&&", "||", ">", ">=", "<", "<=","+", "-", "*", "/", "^", "!", "++", "--", "(", ")" }; // Binary Operators are 0-12
const int Infix_Evaluator::OPERATOR_PRECEDENCE[]= {2, 2, 1, 1, 3, 3, 3, 3, 4, 4, 5, 5, 6, 7, 7, 7, 0, 8};
const int Infix_Evaluator::NUMBER_OF_OPERATORS = 18;

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

bool Infix_Evaluator::PushToOperatorStack(Token opToken)
{
	int tokenIndex = -1, stackIndex = -1;
	int stackPrecedence, tokenPrecedence;
	Token stackOperator;
	int RHS;
	// Check if the operator is valid:
	for (int i = 0; i < NUMBER_OF_OPERATORS; i++)
	{
		if (opToken.data == OPERATOR_LIST[i]){
			tokenIndex = i; break;
		}}
	if (tokenIndex != -1) // If operator exists, then index >= 0
	{
		while (true) // Do the following until a return condition is met
		{
			if (operator_stack.empty()) // Operator Stack is empty
			{
				operator_stack.push_back(opToken);
				return true;
			}
			else // operator_stack was not empty
			{
				// Find precedence of the top of the operator stack
				stackOperator = operator_stack.back();
				for (int j = 0; j < NUMBER_OF_OPERATORS; j++)
				{
					if (stackOperator.data == OPERATOR_LIST[j]){
						stackIndex = j; break;
					}}
				stackPrecedence = OPERATOR_PRECEDENCE[stackIndex];
				tokenPrecedence = OPERATOR_PRECEDENCE[tokenIndex];

				// Compare the precdences of the stack and the token

				if (opToken.data == ")") // We've hit a closing parenthesis
				{
					if (operator_stack.empty()) 
						throw std::logic_error("Closing Parenthesis without Open Parenthesis");
					while (operator_stack.back().data != "(")
					{
							RHS = PopValue(stackOperator);
							if (PushToValueStack(operator_evaluator(stackOperator, RHS))){
								//std::cout << "Evaluated an operator." << std::endl;
								operator_stack.pop_back();
								if (!operator_stack.empty()){
									stackOperator = operator_stack.back();
								}
								else throw std::logic_error("Unbalanced Parenthesis.");
							}
					}
					operator_stack.pop_back();
					return true;
				}
				else if (tokenPrecedence > stackPrecedence || opToken.data == "("){
					// Push token to operator stack and return true
					operator_stack.push_back(opToken);
					return true;}
				else // Stack has a higher precedence than token
				{
					RHS = PopValue(stackOperator); // We'll need a value for the RHS to evaluate the token
					operator_stack.pop_back(); // Since we're about to use the operator from the stack, we pop it off.
					if (PushToValueStack(operator_evaluator(stackOperator, RHS))){
						//std::cout << "Evaluated an operator." << std::endl;
					}
					else
					{
						throw std::logic_error("Something went wrong while pushing a value onto the ValueStack");
					}
				}
			}
		}
	}
	else throw Syntax_Error("Invalid Operator", opToken);
}


Token Infix_Evaluator::PopOperator(void){
	// This function simultaneously removes the top element of the operator stack and returns it
	// This function throws a length error if the stack was empty when this function is called
	if (operator_stack.empty()) throw std::length_error("Operator Stack was empty");
	Token returnToken = operator_stack.back();
	operator_stack.pop_back();
	return returnToken;
}

int Infix_Evaluator::PopValue(Token last_opToken){
	/* This function, like the PopOperator function simultaneously returns and pops the top value
	of value_stack. This function throws length_error if the stack was empty when this function is called. */
	if (value_stack.empty()){
		if (last_opToken.data == "(" || last_opToken.data == ")")
			throw Syntax_Error("Unbalanced Parenthesis", last_opToken);
		else
			throw Syntax_Error("The operator was missing a LHS/RHS", last_opToken);
	}
	int returnValue = value_stack.back();
	value_stack.pop_back();
	return returnValue;
}

int Infix_Evaluator::operator_evaluator(Token opToken, int RHS){
	/* This function is the method by which the infix evaluator figures out what value to push onto the value
	stack. None of us know how to program a stream which evaluates an expression, which might be the fastest
	way of doing this, so we've taken the following option below:
	*/
	//Uniary operators. These do not require a LHS value, we have not taken one from valueStack until after this point.
	std::string op = opToken.data;
	if (op == "!") return !RHS;
	else if (op == "++") return ++RHS;
	else if (op == "--") return --RHS;
	//These two lines will not execute if the operator was Uniary
	int LHS;
	LHS = PopValue(opToken);
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
	else throw Syntax_Error("unrecognized operator", opToken);
}


std::string Infix_Evaluator::evaluate(void){
	int theValue;
	Token opToken;
	while (!operator_stack.empty())
	{
		opToken = PopOperator();
		theValue = PopValue(opToken);
		PushToValueStack(operator_evaluator(opToken, theValue));
	}
	theValue = PopValue(opToken);
	if (value_stack.empty()) 
		return std::to_string(static_cast<long long>(theValue));
	else
		throw std::logic_error("Not enough operators!");
}

//An, Hoang
std::string Infix_Evaluator::evaluate(std::string& expression){
	/* This function uses Parser class to split the expression into tokens. 
	Each token will then be added to value_stack/operator_stack based on its type.
	the function returns the final value of the expression */
	//clear out the 2 stacks first
	value_stack.clear();
	operator_stack.clear();
	std::string answer;
	std::vector<Token> tokens; //temporary vector to store tokens
	try{
		Parser parser(expression); //parse the expression, all tokens will be stored to vector tokens
		tokens = parser.getTokens();
		/*
		//print out tokens for testing
		for (int a = 0; a < tokens.size(); a++){
		std::cout << tokens[a].data << " ";
		}*/
		//adding each token to appropriate stacks
		for (int i =0; i<tokens.size();i++){
			if (isNumber(tokens[i].data)){
				PushToValueStack(stoi(tokens[i].data));
			}
			else{
				PushToOperatorStack(tokens[i]);
			}
		}

		answer = evaluate(); //evaluate whatever left in stacks
	}
	catch (Syntax_Error err) { 
		err.Display();
		return "null";
	}
	catch (std::logic_error err) {
		std::cout << err.what() << std::endl;
		return "null";
	}
	catch (std::length_error err) {
		std::cout << err.what() << std::endl;
		return "null";
	}
	return answer; //this is the final answer
}

//this function checks if a string is a number
bool Infix_Evaluator::isNumber(std::string& token)
{
    std::string::iterator itr = token.begin();
	if (!isdigit(*itr) && token.length() == 1) return false; // just an operator 
	else if	(*itr == '-') ++itr; //check for negative number
    while (itr != token.end() && isdigit(*itr)) ++itr;
    return !token.empty() && itr == token.end();
}

