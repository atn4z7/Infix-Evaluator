#pragma once
#include "Infix_Evaluator.h"
using namespace std;

int main()
{
	string my_string("7 - 7 * 35 + 4 / 6 - 10 ");
	Infix_Evaluator testInfix(my_string);
	
	/*
	cout << "This is a test of compilation and build. " << endl;

	string testString;
	//Infix_Evaluator testInfix;
	cout << "Testing operator Evaluator function" << endl;
	for (int i = 0; i < 15; i++)
	{
		testString = testInfix.OPERATOR_LIST[i];
		testInfix.PushToValueStack(2);
		cout << "2 " << testString << " 5 evaluates to: " << testInfix.operator_evaluator(testString, 5) << endl;
	}

	cout << endl << endl << endl << "Testing push to operator stack" << endl;
	// Test Expression: "3+4*2-1"
	Infix_Evaluator testInfixB;
	testInfixB.PushToValueStack(3);
	testInfixB.PushToOperatorStack("+");
	testInfixB.PushToValueStack(4);
	testInfixB.PushToOperatorStack("*");
	testInfixB.PushToValueStack(2);
	testInfixB.PushToOperatorStack("-");
	testInfixB.PushToValueStack(1);
	int answer = testInfixB.evaluate();
	cout << "The answer to 3 + 4 * 2 - 1 = " << answer << endl;
	*/
	system("pause");
}
