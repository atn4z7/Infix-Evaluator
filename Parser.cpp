#include "Parser.h"
std::vector<Token> Parser::getTokens(){
	return tokens;
}
void Parser::Add(std::string& token){
	Token newtoken = Token(token);
	newtoken.setLength(token.length());
	if (tokens.size() == 0){
		newtoken.setIndex(0) ;
		tokens.push_back(newtoken);
	}
	else{
		newtoken.setIndex( tokens[tokens.size()-1].getIndex()+tokens[tokens.size()-1].getLength());
		tokens.push_back(newtoken);
	}
}
Parser::Parser(std::string& expression){
	//clear out the tokens vector
	tokens.clear();
	std::string tempdigit = "";
	std::string tempoperator = "";
	//remove all spaces from the expression string
	expression.erase(remove(expression.begin(), expression.end(), ' '), expression.end());
	std::cout << expression << std::endl;
	std::string::iterator itr = expression.begin();
	//loop through the expression and check each char
	while (itr != expression.end()){
		if (isdigit(*itr)){ //token is a digit
			//add the last operator to vector first
			if (tempoperator != ""){
				Add(tempoperator);
				tempoperator = "";
			}
			tempdigit+=*itr;
			++itr;
			if (itr == expression.end()){ //if token is at the end of the expression then just add the number to vector
				Add(tempdigit);
				tempdigit = "";
			}
		}
		else{ //token is not a digit
			//add the last number to vector first
			if (tempdigit != ""){
				Add(tempdigit);
				tempdigit = "";
			}
			//below are different cases for different operators
			char current = *itr;
			switch (current)
			{
			case '=':
				tempoperator+=current;
				if (tempoperator.length() == 1){
					++itr;
					if (itr == expression.end()){ //if '=' is at the end
						Add(tempoperator);
						throw Syntax_Error("Invalid Operator!", tokens[tokens.size()-1]);
					}
					Add(tempoperator+*itr);
					if (*itr != '=') //if not "=="
						throw Syntax_Error("Invalid Operator!", tokens[tokens.size()-1]);
				}
				else if (tempoperator.length() == 2){
					Add(tempoperator);
					//check all other valid cases of "*=" (when = is the 2nd char)
					if ((tempoperator != ">=") &&
						(tempoperator != "<=") &&
						(tempoperator != "!=") ){
							throw Syntax_Error("Invalid Operator!", tokens[tokens.size()-1]);
					}
				}
				tempoperator = "";
				break;
			case '|':
				tempoperator+=current;
				if (tempoperator.length() == 2){ //tempoperator is at max length
					Add(tempoperator);
					if (tempoperator != "||"){
						throw Syntax_Error("Invalid Operator!", tokens[tokens.size()-1]);
					}
					tempoperator = "";
				}
				break;
			case '&':
				tempoperator+=current;
				if (tempoperator.length() == 2){ //tempoperator is at max length
					Add(tempoperator);
					if (tempoperator != "&&"){
						throw Syntax_Error("Invalid Operator!", tokens[tokens.size()-1]);
					}
					tempoperator = "";
				}
				break;
			case '/':
			case '%':
			case '*':
				tempoperator+=current;
				++itr;
				if (itr == expression.end()){ //if the operator is at the end
					Add(tempoperator);
					throw Syntax_Error("Missing RHS!", tokens[tokens.size()-1]);
				}
				if (isdigit(*itr)) --itr; // /,%,* is followed by a number
				else{ // /,%,* is followed by operator
					if (*itr == '!' || *itr == '+' || *itr == '-' || *itr == '('){ // /,%,* can only be followed by !,+,- +
						Add(tempoperator);
						--itr;
					}
					else{
						Add(tempoperator+*itr);
						throw Syntax_Error("Invalid Operator!", tokens[tokens.size()-1]);	
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
				//when we meet '(' or ')' we first add anything that is still in tempoperator to vector
				if (tempoperator.length() >= 1){
					//if operator is valid
					Add(tempoperator);
					tempoperator = "";
				}
				//add '(' or ')' to vector
				tempoperator+=current;
				Add(tempoperator);
				tempoperator = "";
				break;
			case '+':
				tempoperator+=current;
				itr++; //the 2nd char
				//there is no 2nd char => throw error
				if (itr == expression.end()){
					Add(tempoperator);
					throw Syntax_Error("Missing a number at the end!", tokens[tokens.size()-1]);
				}
				else if (*itr == '(' || *itr == '!'){ //2nd char is '(' or '!'
					Add(tempoperator);	
					tempoperator = "";
					--itr;
					break;
				}
				if (isdigit(*itr)) --itr; // only 1 +
				else{
					tempoperator+=*itr;
					if (tempoperator == "+-" || tempoperator == "++"){ // +- or ++
						++itr; //the 3rd char
						//there is no 3rd char => throw error
						if (itr == expression.end()){
							if ((tokens.size() > 1 && (tokens[tokens.size()-1].getData() == "++" 
								|| tokens[tokens.size()-1].getData() == "--"||
								tokens[tokens.size()-1].getData() =="!"))){
									tokens[tokens.size()-1].setData(tokens[tokens.size()-1].getData()+tempoperator);
									tokens[tokens.size()-1].setLength(tokens[tokens.size()-1].getData().size());
									throw Syntax_Error("Multiple Unary Operators not supported!", tokens[tokens.size()-1]);
							}
							Add(tempoperator);
							throw Syntax_Error("Missing an operator or a number at the end!", tokens[tokens.size()-1]);
						}
						else if (*itr == '(' || *itr == '!'){ // the 3rd char is '(' / '!'
							
							if (tempoperator.substr(0,1) != tempoperator.substr(1,1)){ //+-( case
								//add + then add -
								throw Syntax_Error("Invalid Operator!", tokens[tokens.size()-1]);
							}
							else{ //case ++
								//check if there is a ++, --, ! before, throw error if there is
								if ((tokens.size() > 1 && (tokens[tokens.size()-1].getData() == "++" 
									|| tokens[tokens.size()-1].getData() == "--"||
									tokens[tokens.size()-1].getData() =="!"))){
									tokens[tokens.size()-1].setData(tokens[tokens.size()-1].getData()+tempoperator);
									tokens[tokens.size()-1].setLength(tokens[tokens.size()-1].getData().size());
									throw Syntax_Error("Multiple Unary Operators not supported!", tokens[tokens.size()-1]);
								}
							}
							Add(tempoperator);
							tempoperator = "";
							--itr;
							break;
						}
						else if (isdigit(*itr)){ // the 3rd char is a digit
							--itr;
							if (tempoperator == "++"){ //++ followed by number
								//check if there is a ++, --, ! before, throw error if there is
								if ((tokens.size() > 1 && (tokens[tokens.size()-1].getData() == "++" 
									|| tokens[tokens.size()-1].getData() == "--"||
									tokens[tokens.size()-1].getData()=="!"))){
									tokens[tokens.size()-1].setData(tokens[tokens.size()-1].getData()+tempoperator);
									tokens[tokens.size()-1].setLength(tokens[tokens.size()-1].getData().size());
									throw Syntax_Error("Multiple Unary Operators not supported!", tokens[tokens.size()-1]);
								}
								Add(tempoperator);
							}
							else{ // +-number (negative number)
								Add(std::string("+"));
								--itr;
							}
						}
						else{ //is operator=> check all valid cases of +**
							tempoperator+=*itr;
							if (tempoperator == "+--"){
								Add(std::string("+"));
								Add(std::string("--"));
							}
							else if (tempoperator == "+++"){
								itr++;
								if (itr == expression.end()){
									Add(tempoperator);
									throw Syntax_Error("Missing a number at the end!", tokens[tokens.size()-1]);
								}
								if (*itr=='+'){
									tempoperator+=*itr;
									Add(tempoperator);
									throw Syntax_Error("Multiple Unary Operators not supported!", tokens[tokens.size()-1]);
								}
								else if (*itr=='!'){
									tempoperator+=*itr;
									Add(std::string("+"));
									Add(std::string("++!"));
									throw Syntax_Error("Multiple Unary Operators not supported!", tokens[tokens.size()-1]);
								}
								else if(isdigit(*itr)||*itr=='('||*itr=='-'){
									Add(std::string("+"));
									Add(std::string("++"));
								}
								itr--;
							}
							else if (tempoperator == "++-"){
								Add(std::string("++"));
								--itr;
							}
							else{
								Add(tempoperator);
								throw Syntax_Error("Invalid Operator!", tokens[tokens.size()-1]);
							}
						}
					}
					else{
						Add(tempoperator);
						throw Syntax_Error("Invalid Operator!", tokens[tokens.size()-1]);
					}
					tempoperator = "";
				}
				break;
			case '-':
				tempoperator+=current;
				++itr; //the 2nd char
				//there is no 2nd char => throw error
				if (itr == expression.end()){
					Add(tempoperator);
					throw Syntax_Error("Missing a number at the end!", tokens[tokens.size()-1]);
				}
				else if (*itr == '(' || *itr == '!' ){ //2nd char is '(' or '!'
					Add(tempoperator);
					tempoperator = "";
					--itr;
					break;
				}
				if (isdigit(*itr)){// only 1 -
					//check for negative number
					--itr; //go back one char to check
					if (itr == expression.begin()){ //negative number at front
						tempdigit+="-";
						tempoperator = "";
					}
					else{
						--itr; //go back one more char
						//if there is an operator before the minus, we've encountered a negative number
						if (!isdigit(*itr) && *itr != ')'){
							tempdigit+="-";
							tempoperator = "";
						}
						++itr;
					}
				}
				else{
					tempoperator+=*itr;
					if (tempoperator == "-+" || tempoperator == "--"){ // -+ or --
						++itr; //the 3rd char
						//there is no 3rd char => throw error
						if (itr == expression.end()){
							Add(tempoperator);
							throw Syntax_Error("Missing an operator or a number at the end!", tokens[tokens.size()-1]);
						}
						else if (*itr == '(' || *itr == '!'){ //the 3rd char is '(' or '!'
							if (tempoperator.substr(0,1) != tempoperator.substr(1,1)){ //-+( case
								Add(tempoperator);
								throw Syntax_Error("Invalid Operator!", tokens[tokens.size()-1]);
							}
							else{ //case --( or --!
								//check if there is a ++, --, ! before, throw error if there is
								if ((tokens.size() > 1 && (tokens[tokens.size()-1].getData() == "++" 
									|| tokens[tokens.size()-1].getData() == "--" 
									|| tokens[tokens.size()-1].getData() == "!"))){
									tokens[tokens.size()-1].setData(tokens[tokens.size()-1].getData()+tempoperator);
									tokens[tokens.size()-1].setLength(tokens[tokens.size()-1].getData().size());
									throw Syntax_Error("Multiple Unary Operators not supported!", tokens[tokens.size()-1]);
								}
							}
							Add(tempoperator);
							tempoperator = "";
							--itr;
							break;
						}
						if (isdigit(*itr)){ //the 3rd char is a digit
							if (tempoperator != "--"){ //+- followed by a number -> wrong format
								Add(tempoperator);
								throw Syntax_Error("Invalid Operator!", tokens[tokens.size()-1]);
							}
							else{ //--number case
								//check if there is a ++, -- before, throw error if there is
								if ((tokens.size() > 1 && (tokens[tokens.size()-1].getData() == "++" || tokens[tokens.size()-1].getData() == "--"))){
									tokens[tokens.size()-1].setData(tokens[tokens.size()-1].getData()+tempoperator);
									tokens[tokens.size()-1].setLength(tokens[tokens.size()-1].getData().size());
									throw Syntax_Error("Multiple Unary Operators not supported!", tokens[tokens.size()-1]);
								}
							}
							Add(tempoperator);
							itr--;
						}
						else{ //is operator => check all valid cases of -**
							if (itr == expression.end()){
								Add(tempoperator);
								throw Syntax_Error("Missing a number at the end!", tokens[tokens.size()-1]);
							}
							tempoperator+=*itr;
							if (tempoperator == "-++"){
								Add(std::string("-"));
								Add(std::string("++"));
							}
							else if (tempoperator == "---"){
								itr++;
								if (itr == expression.end()){
									Add(tempoperator);
									throw Syntax_Error("Missing a number at the end!", tokens[tokens.size()-1]);
								}
								if (*itr=='-'){
									if(tokens.size()==0){
										tempoperator+=*itr;
										Add(tempoperator);
										throw Syntax_Error("Multiple Unary Operators not supported!", tokens[tokens.size()-1]);
									}
									else{
										if (!isdigit(tokens[tokens.size()-1].getData()[0])){
											tempoperator+=*itr;
											Add(tempoperator);
											throw Syntax_Error("Multiple Unary Operators not supported!", tokens[tokens.size()-1]);
										}
									}
								}
								else if (*itr=='!'){
									tempoperator+=*itr;
									Add(std::string("+"));
									Add(std::string("++!"));
									throw Syntax_Error("Multiple Unary Operators not supported!", tokens[tokens.size()-1]);
								}
								Add(std::string("-"));
								Add(std::string("--"));
								itr--;
							}
							else{
								Add(tempoperator);
								throw Syntax_Error("Invalid Operator!", tokens[tokens.size()-1]);
							}
						}
					}
					else{
						Add(tempoperator);
						throw Syntax_Error("Invalid Operator!", tokens[tokens.size()-1]);
					}
					tempoperator = "";
				}
				break;
			case '!':
				//when we meet '!' we first add anything that is still in tempoperator to vector

				if (tempoperator.length() >= 1){
					Add(tempoperator);
					tempoperator = "";
				}
				tempoperator+=current;
				if ((tokens.size() > 1 && (tokens[tokens.size()-1].getData() == "++" 
					|| tokens[tokens.size()-1].getData() == "--" 
					|| tokens[tokens.size()-1].getData() == "!"))){
						tokens[tokens.size()-1].setData(tokens[tokens.size()-1].getData()+tempoperator);
						tokens[tokens.size()-1].setLength(tokens[tokens.size()-1].getData().size());
						throw Syntax_Error("Multiple Unary Operators not supported!", tokens[tokens.size()-1]);
				}
				++itr;
				if (itr == expression.end()){ //if '!' is at the end
					Add(tempoperator);
					throw Syntax_Error("Missing a number at the end!", tokens[tokens.size()-1]);
				}
				if (isdigit(*itr)){ //if '!' is followed by a number
					if (tokens.size() >= 1 && (isdigit(tokens[tokens.size()-1].getData()[0]))){
						Add(tempoperator);
						throw Syntax_Error("Missing a binary operator before this position!", tokens[tokens.size()-1]);
					}
					Add(tempoperator);
					//if before '!', there was a prefix operator then throw error
					tempoperator="";
					--itr;
				}
				else{ // '!' followed by operator
					// '!' can only be followed by +,-,=
					if ( *itr == '('||*itr == '+'||*itr == '-'){
						if (tokens.size() >= 1 && (isdigit(tokens[tokens.size()-1].getData()[0]))){
						Add(tempoperator);
						throw Syntax_Error("Missing a binary operator before this position!", tokens[tokens.size()-1]);
					}
						Add(tempoperator);
						--itr;
					}
					else if (*itr == '='){
						//add "!=" to vector
						Add(tempoperator+*itr);
					}
					else if (*itr == '!'){
						tempoperator+=*itr;
						Add(tempoperator);
						throw Syntax_Error("Multiple Unary Operators not supported!", tokens[tokens.size()-1]);
					}
					else{
						Add(tempoperator);
						tempoperator="";
						tempoperator+=*itr;
						Add(tempoperator);
						throw Syntax_Error("Invalid Operator!", tokens[tokens.size()-1]);
					}
					tempoperator = "";
				}
				break;
			case '^':
				tempoperator+=current;
				++itr;
				if (itr == expression.end()){ //if '^' is at the end
					Add(tempoperator);
					throw Syntax_Error("Missing Exponential!", tokens[tokens.size()-1]);
				}
				if (isdigit(*itr)) --itr; // if '^' is followed by a number
				else{ // '^' is followed by operator
					//'^' can only be followed by !,+,-
					if (*itr == '!' || *itr == '+' || *itr == '-'){
						Add(tempoperator);
						--itr;
					}
					else{
						Add(tempoperator+*itr);
						throw Syntax_Error("Invalid Operator!", tokens[tokens.size()-1]);
					}
					tempoperator = "";
				}
				break;
			default:	//unknown char encountered
				tempoperator+=current;
				Add(tempoperator);
				throw Syntax_Error("Unknown Char Encountered!", tokens[tokens.size()-1]);	
			}
			++itr;
			//before going to the next loop, we check if we are at the end of the expression
			// if so, we add whatever left in tempoperator to vector
			if (itr == expression.end()){
				if (tempoperator != ""){
					Add(tempoperator);
					tempoperator = "";
				}
			}
		}
	}
}
