//============================================================================
// Name        : Syntax_Error.cpp
// Author      : An Nguyen
// Description : Implementation of Syntax_Error class
//============================================================================

#include "Syntax_Error.h"

Syntax_Error::Syntax_Error(){}

Syntax_Error::Syntax_Error(std::string msg, Token token){
	error = msg;
	index = token.getIndex();
	length = token.getLength();
}

void Syntax_Error::Display(){
	//go to the index by displaying multiple " "
	for (int a = 0; a < index; a++){
		std::cout << " ";
	}
	//print out '^' to indicate where the error occurs
	for (int a = 0; a < length; a++){
		std::cout << "^";
	}
	//print out the error message
	std::cout << " " << error << std::endl;
}
