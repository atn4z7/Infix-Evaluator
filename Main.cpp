
#include <fstream>
#include "Infix_Evaluator.h"


int main()
{
	Infix_Evaluator InflixEvaluator;
	std::string expression;
	std::string answer;
	// Input/output files
	std::ifstream fin("input.txt");
	
	// Throw error if can't open input file
	if(!fin)
	{
		std::cerr << "Error opening input file!";
		system("pause");
		return 1;
	}

	while(fin.good())
	{
		getline(fin,expression);
		answer = InflixEvaluator.evaluate(expression);
		if (answer != "null"){
			std::cout << "Answer = " << answer ;
		}
		std::cout << std::endl << std::endl; 
	}

	system("pause");
	// Close files
	fin.close();
	return 0;
}
