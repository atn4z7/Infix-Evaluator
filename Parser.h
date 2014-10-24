#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "Token.h"
#include "Syntax_Error.h"

class Parser{

public:
	//this constructor parses an inflix expression into tokens
	Parser(std::string& expression);
	//function to add and update tokens
	void Add(std::string& token);
	//function to get the vector tokens
	std::vector<Token> getTokens();
private:
	//vector to store splitted tokens
	std::vector<Token> tokens;
};

#endif
