

#include <iostream>
#include <experimental/filesystem>
#include <stdio.h>
#include "SSWriter.hpp"

void SSWriter::writeFile()
{

  handleFileName();

  std::fstream Model;

  Model.open(this->fileName, std::fstream::out);

  writeHeader(Model);

  /**
   * write main block
   */
  if(this->spacing == SSWriter::distance::uneven) 
  {
    // If 0 is not in vector list then add it
    if (!std::count(this->points.begin(), this->points.end(), 0))
    {
      this->points.push_back(0);
    }

    // sort the vector list
    std::sort (points.begin(), points.end());

  }

  writeMain(Model);
 
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
    Model << "const int IPTG = 0;" << std::endl;
    Model << std::endl;
    Model << "formula p_t = 0.99/(0.25*(TetR/(0.0496277915632754*IPTG + 1))*(TetR/(0.0496277915632754*IPTG + 1)) + 1.99);" << std::endl;
    Model << std::endl;
  }

  //if the model has degredation (2,3,4,5) then we need our kd value
  if (this->type >= 2 && this->type <= 5)
  {
    Model << "const double kd = 0.0075;" << std::endl;
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
    Model << "MAX";
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


void SSWriter::writeMain(std::fstream& Model)
{
  switch(this->type)
  {
    case 0:
	    writeORO(Model);
	    break;
    case 1:
	    writeNRO(Model);
	    break;
    case 2:
	    writeODO(Model);
	    break;
    case 3:
	    writeNDO(Model);
	    break;
    case 4:
	    writeOB(Model);
	    break;
    case 5:
	    writeNB(Model);
	    break;
    default:
	    std::cerr << "Model type could not be determined" << std::endl;
	    return;
  }
}


//void SSWriter::writeMainUneven(std::fstream& Model)
//{
//  std::cout << "uneve main block called" << std::endl;
//}

double SSWriter::degredationRange(int lowerInt, int upperInt)
{
  double upper = static_cast<double>(upperInt);
  double lower = static_cast<double>(lowerInt);

  double j = 0;

  for(; upper > lower; upper = upper - 1)
  {
    j += ( 1 / upper );
  }

  return j;
}


double SSWriter::repressionRange(int lowerInt, int upperInt)
{
  double upper = upperInt;
  double lower = lowerInt;

  double j = 0;
  // j gives us the expected 
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


/**************************Main block definistions*****************/

// Old Way Repression only
void SSWriter::writeORO(std::fstream& Model)
{
  //if the object has even thresholds then do this
  if(this->spacing == SSWriter::distance::even)
  {
    for(int j = 0; j < (this->max / this->epsilon); j++)
    {
      Model << " [] TetR=" << (this->epsilon * j) << " -> p_t * " << (10.0 / static_cast<double>(epsilon)) 
            << " : (TetR'=TetR+" << this->epsilon << ");" << std::endl;
    }
  }
  //else the spacing is uneven so execute a separate writer portion
  else
  {
    for(int j = 0; j < this->points.size() - 1; j++)
    {
      int diff = this->points.at(j+1) - this->points.at(j);

      Model << " [] TetR=" << this->points.at(j) << " -> p_t * " << (10.0 / static_cast<double>(diff)) 
	    << " : (TetR'=TetR+" << diff << ");" << std::endl; 
    }
  }
}

// New way Repression Only
void SSWriter::writeNRO(std::fstream& Model)
{
  //if the object has even thresholds then do this
  if(this->spacing == SSWriter::distance::even)
  {
    for(int j = 0; j < (this->max / this->epsilon); j++)
    {
      Model << " [] TetR=" << (this->epsilon * j) << " -> " << SSWriter::repressionRange( j*this->epsilon, (j+1) * epsilon ) 
            << " : (TetR'=TetR+" << this->epsilon << ");" << std::endl;
    }
  }
  //else the spacing is uneven so execute a separate writer portion
  else
  {
    for(int j = 0; j < this->points.size() - 1; j++)
    {
      int curr = this->points.at(j);
      int next = this->points.at(j+1);

      Model << " [] TetR=" << curr << " -> " 
	    << SSWriter::repressionRange( curr, next) 
	    << " : (TetR'=TetR+" << next - curr << ");" << std::endl; 
    }
  }

}

// Old Way Degredation only
void SSWriter::writeODO(std::fstream& Model)
{
  if(this->spacing == SSWriter::distance::even)
  {
    for(int i = 0; i < (max / epsilon); i++)
    {
      Model << " [] TetR=" << (epsilon * (i+1)) << " -> kd * TetR / " 
	    << epsilon << " :(TetR'=TetR-" << epsilon << ");" << std::endl;
    } 
  }
  //else the spacing is uneven so execute a separate writer portion
  else
  {
    for(int j = 1; j < points.size(); j++)
    {
      int diff = points.at(j) - points.at(j-1);

      Model << " [] TetR=" << points.at(j) << " -> kd * TetR / " 
	    << diff << " :(TetR'=TetR-" << diff << ");" << std::endl; 
    }
  }
}

// New Way Degredation only
void SSWriter::writeNDO(std::fstream& Model)
{
  //if the object has even thresholds then do this
  if(this->spacing == SSWriter::distance::even)
  {
    for(int j = 0; j < (max / epsilon); j++)
    {
      Model << " [] TetR=" << (epsilon * (j+1)) << " -> kd / ("
	    << degredationRange( (j * epsilon) , (j+1) * epsilon )
	    << ") :(TetR'=TetR-" << epsilon << ");" << std::endl;
    } 
  }
  //else the object has uneven thresholds
  {
    for(int j = 1; j < points.size(); j++)
    {
      Model << " [] TetR=" << points.at(j) << " -> kd / ("
	    << degredationRange( points.at(j-1) , points.at(j) )
	    << ") :(TetR'=TetR-" << (points.at(j) - points.at(j-1))
	    << ");" << std::endl;
    }
  }
}

// Old way Both
void SSWriter::writeOB(std::fstream& Model)
{
  //if the object has even thresholds then do this
  if(this->spacing == SSWriter::distance::even)
  {
    for(int j = 0; j <= ( max / epsilon); j++)
    {
      //for the first line
      if (j == 0 )
      {
        Model << " [] TetR=" << (epsilon * j) << " -> p_t * "
	      << (10 / static_cast<double>(epsilon)) << " : (TetR'=TetR+"
	      << epsilon << ");" << std::endl;
      }
      //for last line
      else if (j == max / epsilon)
      {
        Model << " [] TetR=" << (epsilon * j) << " -> kd * TetR / " 
              << epsilon << " : (TetR'=TetR-" << epsilon << ");" 
	      << std::endl;
      }
      //for main portion
      else
      {
        Model << " [] TetR=" << (epsilon * j) << " -> p_t * " 
	      << (10.0 / static_cast<double>(epsilon)) << " : (TetR'=TetR+" << epsilon << ") + kd * TetR / "
              << epsilon << " : (TetR'=TetR-" << epsilon << ");" << std::endl;	      
      }
    }
  }
  //else the object has uneven thresholds
  else
  {
    for(int j = 0; j < points.size() - 1; j++)
    {
      //for first line
      if(j == 0)
      {
	int diff = points.at(j+1) - points.at(j);

        Model << " [] TetR=" << points.at(0) << " -> p_t * " 
              << (10.0 / static_cast<double>(diff)) << " : (TetR'=TetR+" << diff << ");" << std::endl;
      }
      //last line
      else if ( j == points.size() - 1)
      {
        int diff = points.at(j) - points.at(j-1);

	Model << " [] TetR=" << points.at(j) << " -> kd * TetR / "
              << diff << " : (TetR'=TetR-" << diff << ");" << std::endl;
      }
      //main portion
      else
      {
	int diff = points.at(j+1) - points.at(j);

        Model << " [] TetR=" << points.at(j) << " -> p_t * " << (10.0 / static_cast<double>(diff))
              << " : (TetR'=TetR+" << diff << ") + kd * TetR / ";
	
	//redefine diff to be the difference from current to one bellow
	diff = points.at(j) - points.at(j-1);

        Model << diff << " : (TetR'=TetR-" << diff << ");" << std::endl;	
      }
    }
  }
}

// New way Both
void SSWriter::writeNB(std::fstream& Model)
{
  //if the object has even thresholds then do this
  if(this->spacing == SSWriter::distance::even)
  {
    for(int j = 0; j <= ( max / epsilon); j++)
    {
      //for the first line
      if (j == 0 )
      {
        Model << " [] TetR=" << (epsilon * j) << " -> ("
	      << repressionRange( j * epsilon, (j+1) * epsilon ) << ") : (TetR'=TetR+"
	      << epsilon << ");" << std::endl;
      }
      //for last line
      else if (j == max / epsilon)
      {
        Model << " [] TetR=" << (epsilon * j) << " -> kd / (" 
              << degredationRange( (j-1) * epsilon, (j) * epsilon) << ") : (TetR'=TetR-" 
	      << epsilon << ");" << std::endl;
      }
      //for main portion
      else
      {
        Model << " [] TetR=" << (epsilon * j) << " -> (" 
	      << repressionRange( j * epsilon, (j+1) * epsilon ) << ") : (TetR'=TetR+" 
	      << epsilon << ") + kd / (" << degredationRange( (j-1) * epsilon, (j) * epsilon)
	      << ") : (TetR'=TetR-" << epsilon << ");" << std::endl;	      
      }
    }
   
  }
  //else the object has uneven thresholds
  else
  {
    for(int j = 0; j < points.size() - 1; j++)
    {
      //for first line
      if(j == 0)
      {
	int diff = points.at(j+1) - points.at(j);

        Model << " [] TetR=" << points.at(0) << " -> (" 
              << repressionRange(points.at(0), points.at(j+1)) << ") : (TetR'=TetR+" 
	      << diff << ");" << std::endl;
      }
      //last line
      else if ( j == points.size() - 1)
      {
        int diff = points.at(j) - points.at(j-1);

	Model << " [] TetR=" << points.at(j) << " -> kd / ("
              << degredationRange(points.at(j-1), points.at(j)) << ") : (TetR'=TetR-" 
	      << diff << ");" << std::endl;
      }
      //main portion
      else
      {
	int diff = points.at(j+1) - points.at(j);

        Model << " [] TetR=" << points.at(j) << " -> " 
              << repressionRange(points.at(j), points.at(j+1))
              << " : (TetR'=TetR+" << diff << ") + kd / (";
	
	//redefine diff to be the difference from current to one bellow
	diff = points.at(j) - points.at(j-1);

        Model << degredationRange(points.at(j-1), points.at(j)) << ") : (TetR'=TetR-" << diff << ");" << std::endl;	
      }
    }

  }

}
/******************End of main block definitions****************/


void SSWriter::handleFileName(void)
{
  //first thing that needs to happen is, we need to check if the 
  //filename was passed in with the .prism extension or not
  //if not then add it
  if(!hasExt(fileName, std::string(".prism")))
  {
    fileName = fileName + std::string(".prism"); 
  }

  //next we need to see if the file name already exists
  //check if file already exists
  if(exists(std::string(fileName)))
  {
    std::cerr << "File " << fileName << " already exists" << std::endl;
    std::cerr << "Deleting..." << std::endl;

    if( remove( fileName.c_str() ) != 0 )
    {
      std::cerr << "Error in deleting file" << std::endl;
    }
  }
}


//static
bool SSWriter::hasExt(std::string file, std::string ext)
{
  int fileLength = file.length() - 1;
  int extLength = ext.length() - 1;

  //if the extension is longer than the filename then lets kill it now
  if (fileLength < extLength) return false;  

  //start at the end of both strings and check if they are the same
  for (; extLength >= 0; extLength--, fileLength--)
  {
    if(file[fileLength] != ext[extLength]) return false;
  } 

  return true;
}
