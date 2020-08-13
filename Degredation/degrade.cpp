
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


#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>

bool is_number(const std::string&);

int main(int argc, char *argv[])
{
	if (argc != 3){
	std::cout << "Correct usage: ./DegredationRange <LowerBound> <UpperBound>" << std::endl;
	throw std::invalid_argument("Wrong number of Arguments");
	}

	if (!is_number(argv[1]) || !is_number(argv[2])){
	std::cout << "One or Both arguments is not a number" << std::endl;
	throw std::invalid_argument("Wrong argument type");
	}

	double upper;
	double lower;


	if (std::stoi(argv[1]) > std::stoi(argv[2])){
		upper = std::stoi(argv[1]);
		lower = std::stoi(argv[2]);
	}
	else{
		upper = std::stoi(argv[2]);
		lower = std::stoi(argv[1]);
	}

	double j = 0;

	for (; upper > lower; upper--)
	{
		j += (1/upper);
	}
	
//	std::cout << "The sum of 1/" << UpperBound << "to 1/" << Lowerbound << "is " << j << std::endl;
//	std::cout << "The 1/" << j << " * 1/" << std::endl;

	std::cout << j << std::endl;
	return 0;
}




bool is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(),
        s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}
