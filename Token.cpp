#include "Token.h"

Token::Token(){}
Token::Token(std::string Data, int Index, int Length){
	data= Data;
	index = Index;
	length = Length;
}
Token& Token::operator=(Token rhs)
{
	data = rhs.data;
	index = rhs.index;
	length = rhs.length;
	return *this;
}
std::string Token::getData(){
	return data;
}
void Token::setData(std::string newData){
	data=newData;
}
int Token::getLength(){
	return length;
}
int Token::getIndex(){
	return index;
}
void Token::setLength(int newLength){
	length=newLength;
}
void Token::setIndex(int newIndex){
	index=newIndex;
}
