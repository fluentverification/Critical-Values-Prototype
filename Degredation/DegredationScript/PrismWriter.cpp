

#include <iostream>
#include <string>
#include <fstream>


#include "PrismWriter.hpp"

//Local Function Prototypes

double degredationRange(int lower, int upper);

void writeFile(std::string FileName, int InitialValue, int epsilon)
{

std::ofstream PrismModel;
PrismModel.open(FileName);

/*******Beginning of Prism Model*******/

PrismModel << "ctmc" << std::endl;
PrismModel << std::endl;
PrismModel << "const double kd = 0.0075 ;" << std::endl;
PrismModel << std::endl;
PrismModel << "const int MAX = " << InitialValue << ";" << std::endl;
PrismModel << std::endl;
PrismModel << "module TetR_def" << std::endl;
PrismModel << std::endl;
PrismModel << "	TetR : [0..MAX] init MAX;" << std::endl;
PrismModel << std:: endl;

/*******Start of Calculated Bulk********/

for(int j = 0; j < (InitialValue / epsilon); j++)
{

PrismModel << "	[] TetR=" << (epsilon * (j+1)) << " -> kd / (" 
     << degredationRange( (j * epsilon) , (j+1) * epsilon )
     << ") :(TetR'=TetR-" << epsilon
     << ");" << std::endl;

}


/********End of Bulk********************/

PrismModel << std::endl << "endmodule" << std::endl;

/*******End of File*********************/

PrismModel.close();

}


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
