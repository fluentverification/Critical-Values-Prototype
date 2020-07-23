
/*********************************
 * In studying the degredation rate of 
 * a single species, we can calculate the 
 * expected time between a certain range.
 * This simple program calculates the 
 * value between to points (in this 
 * case 0 to 20) that can then be 
 * multiplied by the degredation rate
 * to get an expected time
 *
 * For more info, refer to the paper
 * Semi-quantitative Abstraction and
 * Analysis of Chemical Reaction Networks,
 * by Milan Ceska and Jan Kretınski,
 * page 482
 * ******************************/



#include <iostream>

int main()
{

	double j = 0;

	for (double i=20; i > 0; i--)
	{
		j += (1/i);
	}

	std::cout << "The value is " << j << std::endl;

	return 0;
}
