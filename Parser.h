#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include <algorithm>
#include "Token.h"
#include "Syntax_Error.h"

class Parser{

public:
	Parser();
	//this constructor parses an inflix expression into tokens as well as throws syntax errors if there are any
	Parser(std::string& expression);
	//function to add and update tokens in the tokens vector
	void Add(std::string& token);
	//function to get the vector tokens
	std::vector<Token> getTokens();
private:
	//vector to store splitted tokens
	std::vector<Token> tokens;
};

#endif
