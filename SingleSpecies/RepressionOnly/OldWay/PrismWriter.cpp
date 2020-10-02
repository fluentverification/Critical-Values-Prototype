

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

#include "PrismWriter.hpp"

//Local Function Prototypes

double degredationRange(int lower, int upper);


/****************************************/
//Beginning of epsilon based scriptor
/****************************************/
void writeFile(std::string FileName, int InitialValue, int epsilon)
{

std::ofstream PrismModel;
PrismModel.open(FileName);

/*******Beginning of Prism Model*******/

PrismModel << "ctmc" << std::endl;
PrismModel << std::endl;
PrismModel << "const int IPTG = 0 ;" << std::endl;
PrismModel << std::endl;
PrismModel << "formula p_t = 0.99/(0.25*(TetR/(0.0496277915632754*IPTG + 1))*(TetR/(0.0496277915632754*IPTG + 1)) + 1.99);" << std::endl;
PrismModel << std::endl;
PrismModel << "const int MAX = " << InitialValue << ";" << std::endl;
PrismModel << std::endl;
PrismModel << "module TetR_def" << std::endl;
PrismModel << std::endl;
PrismModel << "	TetR : [0..MAX] init 0;" << std::endl;
PrismModel << std::endl;

/*******Start of Calculated Bulk********/

for(int j = 0; j < (InitialValue / epsilon); j++)
{

PrismModel << "	[] TetR=" << (epsilon * j) << " -> p_t : " 
     << "(TetR'=TetR+" << epsilon
     << ");" << std::endl;

}


/********End of Bulk********************/

PrismModel << std::endl << "endmodule" << std::endl;

/*******End of File*********************/

PrismModel.close();
}
/***************************************/
//End of Epsilon based script writer
/***************************************/




/*********************************/
//Irregular variable script writer
/*********************************/
void writeFile(std::string filename, std::vector<int> pt)
{

if (!std::count(pt.begin(), pt.end(), 0)) pt.push_back(0);

std::sort (pt.begin(), pt.end());

//File start
std::ofstream PrismModel;
PrismModel.open(filename);

//std::cout << pt.back() << std::endl;

/*******Beginning of Prism Model*******/

PrismModel << "ctmc" << std::endl;
PrismModel << std::endl;
PrismModel << "const int IPTG = 0 ;" << std::endl;
PrismModel << std::endl;
PrismModel << "formula p_t = 0.99/(0.25*(TetR/(0.0496277915632754*IPTG + 1))*(TetR/(0.0496277915632754*IPTG + 1)) + 1.99);" << std::endl;
PrismModel << std::endl;
PrismModel << "const int MAX = " <<  pt.back()<< ";" << std::endl;
PrismModel << std::endl;
PrismModel << "module TetR_def" << std::endl;
PrismModel << std::endl;
PrismModel << "	TetR : [0..MAX] init 0;" << std::endl;
PrismModel << std::endl;


/*******Start of Calculated Bulk********/

for(int j = 0; j < pt.size() - 1; j++)
{

PrismModel << "	[] TetR=" << pt.at(j) << " -> p_t : " 
     << "(TetR'=TetR+" << (pt.at(j+1) - pt.at(j))
     << ");" << std::endl;

}

/********End of Bulk********************/

PrismModel << std::endl << "endmodule" << std::endl;

/*******End of File*********************/

PrismModel.close();

//for (auto it = pt.begin(); it != pt.end(); it++) 
//        std::cout << *it << " "; 

}
/********************************/
//End of Irregular variable script
/********************************/



/*******************************/
//Local Function Definitions


double degredationRange(int lowerInt, int upperInt)
{

	double upper = upperInt;
	double lower = lowerInt;

	double j = 0;

	for (; upper > lower; upper--)
	{
		j += (1/upper);
	}

	return j;
}
