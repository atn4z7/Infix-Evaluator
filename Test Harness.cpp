#pragma once
#include "Infix Evaluator.h"

using namespace std;

int main()
{
	cout << "This is a test of compilation and build. " << endl;

	string testString = "2+2^2*3";
	Infix_Evaluator testInfix(testString);
	cout << "The expression " << testString << " evaluates to: " << testInfix.r_int << endl;
	system("pause");
}