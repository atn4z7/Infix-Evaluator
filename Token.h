#ifndef TOKEN_H
#define TOKEN_H

#include <string>

class Token{
public:
	Token();
	Token(std::string Data,  int Index=0, int Length=0);
	//overloading assignment operator
	Token& operator=(Token rhs);
	//data contains the operator string
	std::string data;
	//the index of the operator string in the original expression
	int index;
	//the length of the operator string
	int length;
};



#endif
