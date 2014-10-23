#pragma once
#include "Infix Evaluator.h"

using namespace std;

int main()
{
	cout << "This is a test of compilation and build. " << endl;

	string testString = "13*19+++-9";
	Infix_Evaluator testInfix(testString);
	system("pause");
}