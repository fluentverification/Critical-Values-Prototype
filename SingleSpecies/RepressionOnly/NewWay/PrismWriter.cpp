

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

#include "PrismWriter.hpp"

//Local Function Prototypes

double repressionRange(int lower, int upper);

double p_t(int value);

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
//PrismModel << "const int IPTG = 0 ;" << std::endl;
//PrismModel << std::endl;
//PrismModel << "formula p_t = 0.99/(0.25*(TetR/(0.0496277915632754*IPTG + 1))*(TetR/(0.0496277915632754*IPTG + 1)) + 1.99);" << std::endl;
//PrismModel << std::endl;
PrismModel << "const int MAX = " << InitialValue << ";" << std::endl;
PrismModel << std::endl;
PrismModel << "module TetR_def" << std::endl;
PrismModel << std::endl;
PrismModel << "	TetR : [0..MAX] init 0;" << std::endl;
PrismModel << std::endl;

/*******Start of Calculated Bulk********/

for(int j = 0; j < (InitialValue / epsilon); j++)
{

PrismModel << "	[] TetR=" << j * epsilon << " -> " << repressionRange( j * epsilon, (j+1) * epsilon ) << " : " 
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
void writeFile(std::string filename, std::vector<int> vt)
{

if (!std::count(vt.begin(), vt.end(), 0)) vt.push_back(0);

std::sort (vt.begin(), vt.end());

//File start
std::ofstream PrismModel;
PrismModel.open(filename);

//std::cout << vt.back() << std::endl;

/*******Beginning of Prism Model*******/

PrismModel << "ctmc" << std::endl;
PrismModel << std::endl;
//PrismModel << "const int IPTG = 0 ;" << std::endl;
//PrismModel << std::endl;
//PrismModel << "formula p_t = 0.99/(0.25*(TetR/(0.0496277915632754*IPTG + 1))*(TetR/(0.0496277915632754*IPTG + 1)) + 1.99);" << std::endl;
//PrismModel << std::endl;
PrismModel << "const int MAX = " <<  vt.back()<< ";" << std::endl;
PrismModel << std::endl;
PrismModel << "module TetR_def" << std::endl;
PrismModel << std::endl;
PrismModel << "	TetR : [0..MAX] init 0;" << std::endl;
PrismModel << std::endl;


/*******Start of Calculated Bulk********/

for(int j = 0; j < vt.size() - 1; j++)
{

PrismModel << "	[] TetR=" << vt.at(j) << " -> " << repressionRange(vt.at(j), vt.at(j+1)) << " : " 
     << "(TetR'=TetR+" << (vt.at(j+1) - vt.at(j))
     << ");" << std::endl;

}

/********End of Bulk********************/

PrismModel << std::endl << "endmodule" << std::endl;

/*******End of File*********************/

PrismModel.close();

//for (auto it = vt.begin(); it != vt.end(); it++) 
//        std::cout << *it << " "; 

}
/********************************/
//End of Irregular variable script
/********************************/



/*******************************/
//Local Function Definitions


double repressionRange(int lowerInt, int upperInt)
{

	double upper = upperInt;
	double lower = lowerInt;

	double j = 0;

	for (; lower < upper; lower++)
	{
		//sumation of expected time
		j += (1/p_t(lower));
	}
	// return 1/expected time gives us rate
	return 1/j;
}


double p_t(int value) {
	int IPTG = 0;
	return 0.99/(0.25*(value/(0.0496277915632754*IPTG + 1))*(value/(0.0496277915632754*IPTG + 1)) + 1.99);
}
