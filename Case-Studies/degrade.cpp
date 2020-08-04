
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

#define UpperBound 10 

#define LowerBound 0

int main()
{

	double j = 0;

	for (double i=UpperBound; i > LowerBound; i--)
	{
		j += (1/i);
	}
	
//	std::cout << "The sum of 1/" << UpperBound << "to 1/" << Lowerbound << "is " << j << std::endl;
//	std::cout << "The 1/" << j << " * 1/" << std::endl;

	std::cout << j << std::endl;
	return 0;
}
