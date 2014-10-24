#include "Parser.h"


std::vector<Token> Parser::getTokens(){
	return tokens;
}

void Parser::Add(std::string& token){
	Token newtoken = Token(token);
	newtoken.length = token.length();
	if (tokens.size() == 0){
		newtoken.index = 0;
		tokens.push_back(newtoken);
	}
	else{
		newtoken.index = tokens[tokens.size()-1].index+tokens[tokens.size()-1].length;
		tokens.push_back(newtoken);
	}
}

Parser::Parser(std::string& expression){
	tokens.clear();
	std::string tempdigit = "";
	std::string tempoperator = "";
	//remove all spaces first
	expression.erase(remove(expression.begin(), expression.end(), ' '), expression.end());
	std::cout << expression << std::endl;
	std::string::iterator itr = expression.begin();
	while (itr != expression.end()){
		if (isdigit(*itr)){ //token is a digit
			//add the last operator to vector first
			if (tempoperator != ""){
				Add(tempoperator);
				tempoperator = "";
			}
			tempdigit+=*itr;
			++itr;
			if (itr == expression.end()){ //if token is at the end of the expression then just add it to vector
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

			char current = *itr;
			switch (current)
			{
			case '=': 
				tempoperator+=current;
				if (tempoperator.length() == 1){
					++itr;
					if (itr == expression.end()){
						Add(tempoperator);
						throw Syntax_Error("Invalid Operator!", tokens[tokens.size()-1]);
					}
					Add(tempoperator+*itr);
					if (*itr != '=')
						throw Syntax_Error("Invalid Operator!", tokens[tokens.size()-1]);
				}
				else if (tempoperator.length() == 2){ 
					Add(tempoperator);
					//check all other valid cases of "_="
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
				if (tempoperator.length() == 2){ //full
					Add(tempoperator);
					if (tempoperator != "||"){
						throw Syntax_Error("Invalid Operator!", tokens[tokens.size()-1]);
					}
					tempoperator = "";
				}
				break;
			case '&':
				tempoperator+=current;
				if (tempoperator.length() == 2){ //full
					Add(tempoperator);
					if (tempoperator != "&&"){
						throw Syntax_Error("Invalid Operator!", tokens[tokens.size()-1]);
					}
					tempoperator = "";
				}
				break;
			case '+': // * + 3 will be pushed to vector fine (*,+,3)  even LHS is missing
				tempoperator+=current;
				++itr; //the 2nd char
				//there is no 2nd char => throw error
				if (itr == expression.end()){
					Add(tempoperator);
					throw Syntax_Error("Invalid Operator!", tokens[tokens.size()-1]);
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
							Add(tempoperator);
							throw Syntax_Error("Invalid Operator!", tokens[tokens.size()-1]);
						}
						else if (*itr == '(' || *itr == '!'){ // the 3rd char is '(' / '!'
							Add(tempoperator);
							if (tempoperator.substr(0,1) != tempoperator.substr(1,1)){ //+-( case
								//add + then add -
								throw Syntax_Error("Invalid Operator!", tokens[tokens.size()-1]);
							}
							else{ //case ++
								//check if there is a ++, -- before, throw error if there is
								if ((tokens.size() > 1 && (tokens[tokens.size()-2].data == "++" || tokens[tokens.size()-2].data == "--"))){
									throw Syntax_Error("Multiple Prefix Operators not supported!", tokens[tokens.size()-2]);
								}
							}
							tempoperator = "";
							--itr;
							break;
						}
						else if (isdigit(*itr)){ // the 3rd char is a digit
							--itr;
							if (tempoperator == "++"){ //++number
								Add(tempoperator);
								//check if there is a ++, --, ! before, throw error if there is
								if ((tokens.size() > 1 && (tokens[tokens.size()-2].data == "++" 
									|| tokens[tokens.size()-2].data == "--" 
									|| tokens[tokens.size()-2].data == "!"))){
									throw Syntax_Error("Multiple Prefix Operators not supported!", tokens[tokens.size()-2]);
								}
							}
							else{ // +-number (negative number)
								Add(std::string("+"));
								--itr;
							}
							
						}
						else{ //is operator
							tempoperator+=*itr;
							if (tempoperator == "+--"){
								Add(std::string("+"));
								Add(std::string("--"));
							}
							else if (tempoperator == "+++"){
								Add(std::string("+"));
								Add(std::string("++"));
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
					throw Syntax_Error("Invalid Operator!", tokens[tokens.size()-1]);
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
						if (!isdigit(*itr)){//check if there is an operator before the minus
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
							throw Syntax_Error("Invalid Operator!", tokens[tokens.size()-1]);
						}
						else if (*itr == '(' || *itr == '!'){ //the 3rd char is '(' or '!'
							Add(tempoperator);
							if (tempoperator.substr(0,1) != tempoperator.substr(1,1)){  //-+( case
								throw Syntax_Error("Invalid Operator!", tokens[tokens.size()-1]);
							}
							else{ //case --( or --!
								//check if there is a ++, --, ! before, throw error if there is
								if ((tokens.size() > 1 && (tokens[tokens.size()-2].data == "++" 
									|| tokens[tokens.size()-2].data == "--" 
									|| tokens[tokens.size()-2].data == "!"))){
									throw Syntax_Error("Multiple Prefix Operators not supported!", tokens[tokens.size()-2]);
								}
							}
							tempoperator = "";
							--itr;
							break;
						}
						if (isdigit(*itr)){ //the 3rd char is a digit
							Add(tempoperator);
							if (tempoperator != "--") //+-number -> wrong format
								throw Syntax_Error("Invalid Operator!", tokens[tokens.size()-1]);
							else{ //--number case
								//check if there is a ++, -- before, throw error if there is
								if ((tokens.size() > 1 && (tokens[tokens.size()-2].data == "++" || tokens[tokens.size()-2].data == "--"))){
									throw Syntax_Error("Multiple Prefix Operators not supported!", tokens[tokens.size()-2]);
								}
							}
							itr--;
						}
						else{ //is operator
							tempoperator+=*itr;
							if (tempoperator == "-++"){
								Add(std::string("-"));
								Add(std::string("++"));
							}
							else if (tempoperator == "---"){
								Add(std::string("-"));
								Add(std::string("--"));
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
			case '/': 
			case '%':
			case '*': 
				tempoperator+=current;
				++itr;
				if (itr == expression.end()){
					Add(tempoperator);
					throw Syntax_Error("Missing RHS", tokens[tokens.size()-1]);
				}
				if (isdigit(*itr)) --itr;
				else{ // * followed by operator
					if (*itr == '!' || *itr == '+' || *itr == '-' || *itr == '('){ //* can be followed by !,+,- +
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
				if (tempoperator.length() >= 1){ //when we meet ! we first add anything that is still in tempoperator to vector
					//if op is valid
					Add(tempoperator);
					tempoperator = "";
				}
				tempoperator+=current;
				Add(tempoperator);
				tempoperator = "";
				break;
			case '!':
				if (tempoperator.length() >= 1){ //when we meet ! we first add anything that is still in tempoperator to vector
					Add(tempoperator);
					tempoperator = "";
				}
				tempoperator+=current;
				++itr;
				if (itr == expression.end()){
					Add(tempoperator);
					throw Syntax_Error("Invalid Operator!", tokens[tokens.size()-1]);
				}
				if (isdigit(*itr)){ 
					Add(tempoperator);
					if ((tokens.size() > 1 && (tokens[tokens.size()-2].data == "++" 
						|| tokens[tokens.size()-2].data == "--" 
						|| tokens[tokens.size()-2].data == "!"))){
						throw Syntax_Error("Multiple Prefix Operators not supported!", tokens[tokens.size()-2]);
					}
					tempoperator="";
					--itr;
				}
				else{ // ! followed by operator - * can be followed by !,+,-,= 
					if (*itr == '!' || *itr == '+' || *itr == '-' || *itr == '('){
						Add(tempoperator);
						--itr;
					}
					else if (*itr == '='){
						//add "!=" to vector
						Add(tempoperator+*itr);
					}
					else{
						Add(tempoperator+*itr);
						throw Syntax_Error("Invalid Operator!", tokens[tokens.size()-1]);
					}
					tempoperator = "";
				}
				break;
			case '^':
				tempoperator+=current;
				++itr;
				if (itr == expression.end()){
					Add(tempoperator);
					throw Syntax_Error("Missing Exponential!", tokens[tokens.size()-1]);
				}
				if (isdigit(*itr)) --itr;
				else{ // ^ followed by operator - ^ can be followed by !,+,-
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
				//default:	
			}
			++itr;
			if (itr == expression.end()){
				if (tempoperator != ""){
					Add(tempoperator);
					tempoperator = "";
				}
			}
		}
	}
}
