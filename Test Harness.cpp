#pragma once
#include "Infix Evaluator.h"

using namespace std;

int main()
{
	cout << "This is a test of compilation and build. " << endl;

	string testString = "++++1";
	bool factChecker = (5 + 0 > !(5 - 2)) && (2 + 1) == 3;
	Infix_Evaluator testInfix(testString);
	cout << "The expression " << testString << " evaluates to: " << testInfix.r_int << endl;
	cout << "FactChecker says: " << factChecker << endl;
	system("pause");
}