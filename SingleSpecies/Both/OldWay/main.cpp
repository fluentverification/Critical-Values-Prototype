
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

//system includes

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

//Program includes

#include "PrismWriter.hpp"


//Local funciton prototypes
bool is_number(const std::string&);

//Program Start
int main(int argc, char *argv[])
{

	if (argc < 4 ){
	std::cout << "Correct usage: ./<program> <prism model name> <values>" << std::endl;
	return -1;
	}
		
	std::string filename = argv[1];
	if (filename.find(".prism") == std::string::npos){
		filename = filename + ".prism";
	}

	if (argc == 4 ){

		//if(argc != 5){
		//std::cout << "Correct usage: ./<program> <prism model name> 0 <initial value> <epsilon>" << std::endl;
		//return -1;
		//}

		if (!is_number(argv[2]) || !is_number(argv[3])){
		std::cout << "Third and Fourth arguments must be integers" << std::endl;
		return -1;
		}

		if ((std::stoi(argv[2]) % std::stoi(argv[3])) != 0){
		std::cout << "Epsilon must be a factor of the initial value" << std::endl;
		return -1;
		}

		writeFile(filename, std::stoi(argv[2]), std::stoi(argv[3]));	

	}

	if (argc >= 5 ){
		
		//if(argc <= 5){
		//std::cout << "Correct usage: ./<program> <prism model name> 1 <greatest threshold> ... <0>" << std::endl;
		//return -1;
		//}

		std::vector<int> pt;

		for(int i = (argc - 1); i > 1 ; i--){
		
			//std::cout << "Inserting Value: " <<  std::stoi(argv[i]) << "to Vector" << std::endl;
			pt.push_back(std::stoi(argv[i]));		
		}


		writeFile(filename, pt);	

	}

	return 0;
}






//Local function definitions

bool is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(),
        s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}
