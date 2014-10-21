
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
#include <math.h>
using namespace std;

vector<string> Parser(string& expression){
	vector<string> result;
	char token;
	string tempdigit = "";
	string tempoperator = "";
	string::iterator itr = expression.begin();
	while (itr != expression.end(){
		if (*itr.isdigit()){ //is digit
			//for operator
			if (!tempoperator == ""){
				result.push_back(tempoperator);
				tempoperator = "";
			}
			tempdigit+=*itr;
			++itr;
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
				if (tempoperator.length == 2){ //full
					if ((tempoperator != ">=") || //check all valid of "*="
						(tempoperator != "<=") ||
						(tempoperator != "==") ||
						(tempoperator != "!=") ){
						result.push_back("Error Here ->");
					}
					result.push_back(tempoperator);
					tempoperator = "";
				}
				break;
			case '|':
				tempoperator+=current;
				if (tempoperator.length == 2){ //full
					if (tempoperator != "||"){
						result.push_back("Error Here ->");
					}
					result.push_back(tempoperator);
					tempoperator = "";
				}
				break;
			case '&':
				tempoperator+=current;
				if (tempoperator.length == 2){ //full
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
				if (*itr.isdigit()) --itr; // only 1 +
				else{
					tempoperator+=*itr;
					if (tempoperator == "+-" || tempoperator == "++"){ // +- or ++
						++itr; //the 3rd char
						if (*itr.isdigit()){
							if (tempoperator != "++") //+-number -> wrong format
								result.push_back("Error Here ->");
							result.push_back(tempoperator);
							itr--;
						}
						else{ //is operator
							tempoperator+=*itr;
							if (tempoperator == "+--"){
								result.push_back('+');
								result.push_back("--");
							}
							else if (tempoperator == "+++"){
								result.push_back('+');
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
				if (*itr.isdigit()) --itr; // only 1 +
				else{
					tempoperator+=*itr;
					if (tempoperator == "-+" || tempoperator == "--"){ // +- or ++
						++itr; //the 3rd char
						if (*itr.isdigit()){
							if (tempoperator != "--") //+-number -> wrong format
								result.push_back("Error Here ->");
							result.push_back(tempoperator);
							itr--;
						}
						else{ //is operator
							tempoperator+=*itr;
							if (tempoperator == "-++"){
								result.push_back('-');
								result.push_back("++");
							}
							else if (tempoperator == "---"){
								result.push_back('-');
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
			case '*': //not count LHS is missing case
				tempoperator+=current;
				++itr;
				if (*itr.isdigit()) --itr;
				else{ // * followed by operator
					tempoperator+=*itr;
					if (*itr != '+' && *itr != '-'){ //not count *!1 case
						result.push_back("Error Here ->");
						result.push_back(tempoperator);
					}
					else{ //*+ or *- case
						//check the next char
						++itr;
						tempoperator+=*itr;
						if (tempoperator != "*++" && tempoperator != "*--"){
							result.push_back("Error Here ->");
							result.push_back(tempoperator);
						}
						else{
							result.push_back(tempoperator.substr(0,1));
							result.push_back(tempoperator.substr(1,2));
						}
					}
					tempoperator = "";
				}
				break;
			case '>':
			case '<':
				tempoperator+=current;
				break;
			case '!':
				tempoperator+=current;
				++itr;
				if (*itr.isdigit()) --itr;
				else{ // * followed by operator
					tempoperator+=*itr;
					if (*itr != '='){ //not count !++ !-- case
						result.push_back("Error Here ->");
						result.push_back(tempoperator);
					}
					else{
						result.push_back(tempoperator.substr(0,1);
						--itr;
					}
					tempoperator = "";
				}
				break;
			case '^':

			default:

			}
			
			++itr;

		}
	}




}
