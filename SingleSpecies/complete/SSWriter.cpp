

#include <iostream>
#include <experimental/filesystem>
//#include <fstream>
#include "SSWriter.hpp"

void SSWriter::writeFile()
{


  //check if file already exists
  if(exists(this->fileName))
  {
    std::cerr << "File already exists" << std::endl;
    return;
  }

  std::fstream Model;

  Model.open(this->fileName, std::fstream::app);

  writeHeader(Model);

  /**
   * write main block
   */
  if(this->spacing == SSWriter::distance::even)
  {
    writeMainEven(Model);
  }
  else
  {
    writeMainUneven(Model);
  }
 
  writeEnd(Model);

  Model.close();
}

void SSWriter::writeHeader(std::fstream& Model)
{
  //std::ofstream Model;
  //Model.open(this->fileName, std::fstream::app);
  Model << "ctmc" << std::endl;
  Model << std::endl;
  
  //if the model is oldway and repression (0,4) then include the formula
  if (this->type == 0 || this->type == 4)
  {
    Model << "const int ";
    Model << "IPTG = 0;" << std::endl;
    Model << std::endl;
    Model << "formula p_t = 0.99/(0.25*(TetR/(0.0496277915632754*IPTG + 1))*(TetR/(0.0496277915632754*IPTG + 1)) + 1.99);" << std::endl;
    Model << std::endl;
  }

  Model << "const int MAX = " << this->max << ";" << std::endl;
  Model << std::endl;
  Model << "module TetR_def" << std::endl;
  Model << std::endl;
  Model << " TetR : [0..MAX] init ";

  //if the model is Repression Only(0 or 1) then start at 0
  if (this->type == 0 || this->type == 1)
  {
    Model << "0";
  } 
  //else if the model is Degredation Only(2 or 3) then start at max
  else
  if (this->type == 2 || this->type == 3)
  {
    Model << this->max;
  }
  //else the model is both so start in the middle between 0-max
  else
  {
    Model << this->max / 2;
  }

  Model << ";\n" << std::endl;

  //Model.close();
}


void SSWriter::writeEnd(std::fstream& Model)
{
  //std::ofstream Model;
  //Model.open(this->fileName, std::fstream::app);

  Model << std::endl << "endmodule" << std::endl;

  //Model.close();
}


void SSWriter::writeMainEven(std::fstream& Model)
{
  std::cout << "even main block called" << std::endl;
  
  switch(this->type)
  {
    case 0:
	    break;
    case 1:
	    break;
    case 2:
	    break;
    case 3:
	    break;
    case 4:
	    break;
    case 5:
	    break;
    default:
	    std::cerr << "Model type could not be determined" << std::endl;
  }

}


void SSWriter::writeMainUneven(std::fstream& Model)
{
  std::cout << "uneven main block called" << std::endl;
}

double SSWriter::repressionRange(int lowerInt, int upperInt)
{
  double upper = upperInt;
  double lower = lowerInt;

  double j = 0;

  for(; lower < upper; lower++)
  {
    j += (1/p_t(lower));
  }
  //return 1/expected time gives us rate
  
  return 1/j;
}

double SSWriter::p_t(int value)
{
  int IPTG = 0;
  return 9.9/(0.25*(value/(0.0496277915632754*IPTG + 1))*(value/(0.0496277915632754*IPTG + 1)) + 1.99);
}
