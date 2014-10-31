//============================================================================
// Name        : Syntax_Error.h
// Author      : An Nguyen
// Description : Syntax_Error class header file
//============================================================================

#ifndef SYNTAX_ERROR_H
#define SYNTAX_ERROR_H

#include "Token.h"
#include <iostream>


class Syntax_Error{
	public:
		Syntax_Error();
		//this constructor takes a token in order to print out the error correctly based on the token index and length
		Syntax_Error(std::string msg, Token token);
		//function to print out where the error occurs in the expression
		void Display();
	private:
		std::string error;
		//index of the token in the original expression
		int index;
		//length of the token
		int length;
};

#endif
