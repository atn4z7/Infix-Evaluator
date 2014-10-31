//============================================================================
// Name        : Token.h
// Author      : An Nguyen
// Description : Token class header file
//============================================================================

#ifndef TOKEN_H
#define TOKEN_H
#include <string>

class Token{

public:
	Token();
	Token(std::string Data, int Index=0, int Length=0);
	//overloading assignment operator
	Token& operator=(Token rhs);
	//setters and getters
	std::string getData();
	int getLength();
	int getIndex();
	void setData(std::string newData);
	void setLength(int newLength);
	void setIndex(int newIndex);
private:
	//data contains the operator string
	std::string data;
	//the index of the operator string in the original expression
	int index;
	//the length of the operator string
	int length;
};

#endif
