
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

