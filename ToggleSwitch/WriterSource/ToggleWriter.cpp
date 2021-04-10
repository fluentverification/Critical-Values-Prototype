#include <iostream>
#include <experimental/filesystem>
#include <stdio.h>
#include "ToggleWriter.hpp"

bool contains (std::vector<int> points, int test)
{
  for (int i = 0; i < points.size(); i++)
  {
    if (test == points.at(i)) return true;
  }
  return false;
}

void ToggleWriter::writeFile()
{

  handleFileName();

  std::fstream Model;

  Model.open(this->fileName, std::fstream::out);

  writeHeader(Model);

  /**
   * write main block
   */
  // Sort the vectors contating the points
  if(this->TetR_spacing == ToggleWriter::distance::uneven) 
  {
    // If 0 is not in vector list then add it
    if (!std::count(this->TetR_points.begin(), this->TetR_points.end(), 0))
    {
      this->TetR_points.push_back(0);
    }

    // remove duplicates from list
    std::vector<int> temp = TetR_points;
    TetR_points.clear();
    for (int i = 0; i < temp.size(); i++)
    {
      if (!contains( TetR_points, temp.at(i) ))
      {
        TetR_points.emplace_back( temp.at(i) );
      }
    }
    
    // sort the vector list
    std::sort (TetR_points.begin(), TetR_points.end());

  }

  // Do the same for LacI
  if(this->LacI_spacing == ToggleWriter::distance::uneven) 
  {
    // If 0 is not in vector list then add it
    if (!std::count(this->LacI_points.begin(), this->LacI_points.end(), 0))
    {
      this->LacI_points.push_back(0);
    }

    // remove duplicates from list
    std::vector<int> temp = LacI_points;
    LacI_points.clear();
    for (int i = 0; i < temp.size(); i++)
    {
      if (!contains( LacI_points, temp.at(i) ))
      {
        LacI_points.emplace_back( temp.at(i) );
      }
    }
    
    // sort the vector list
    std::sort (LacI_points.begin(), LacI_points.end());

  }

  Model << "module TetR_def" << std::endl;
  Model << std::endl;
  Model << "  TetR : [0..MAXTetR] init 0;" << std::endl;
  Model << std::endl;

  //write TetR module
  if(TetR_type == ToggleWriter::method::JetC)
  {
    write_TetR_JetC(Model);
  }
  else
  {
    write_TetR_Ceska(Model);
  }
  
  Model << std::endl;
  Model << "endmodule" << std::endl;
  
  Model << std::endl;

  Model << "module LacI_def" << std::endl;
  Model << std::endl;
  Model << "  LacI : [0..MAXLacI] init 0;" << std::endl;
  Model << std::endl;
  
  //Write LacI module
  if(LacI_type == ToggleWriter::method::JetC)
  {
    write_LacI_JetC(Model);
  }
  else
  {
    write_LacI_Ceska(Model);
  }

  Model << std::endl;
  Model << "endmodule" << std::endl;
  
  Model.close();
}

void ToggleWriter::writeHeader(std::fstream& Model)
{
  Model << "ctmc" << std::endl;
  Model << std::endl;
  
  //if TetR is JetC then include the formula
  if (this->TetR_type == 0)
  {
    Model << "const int aTc = 0;" << std::endl;
    Model << std::endl;
    Model << "formula p_l = 0.99/(0.25*(LacI/(0.0496277915632754*aTc + 1))*(LacI/(0.0496277915632754*aTc + 1)) + 1.99);" << std::endl;
    Model << std::endl;
  }

  //TetR max
  Model << "const int MAX_TetR = " << this->TetR_max << ";" << std::endl;
  Model << std::endl;

  //if LacI is JetC then include the formula
  if (this->LacI_type == 0)
  {
    Model << "const int IPTG = 0;" << std::endl;
    Model << std::endl;
    Model << "formula p_t = 0.99/(0.25*(TetR/(0.0496277915632754*IPTG + 1))*(TetR/(0.0496277915632754*IPTG + 1)) + 1.99);" << std::endl;
    Model << std::endl;
  }

  //LacI max
  Model << "const int MAX_LacI = " << this->LacI_max << ";" << std::endl;
  Model << std::endl;

  Model << "const double kd = 0.0075;" << std::endl;
  Model << std::endl;

}


//void ToggleWriter::writeEnd(std::fstream& Model)
//{
//  //std::ofstream Model;
//  //Model.open(this->fileName, std::fstream::app);
//
//  Model << std::endl << "endmodule" << std::endl;
//
//  //Model.close();
//}

double ToggleWriter::degredationRange(int lowerInt, int upperInt)
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


double ToggleWriter::repressionRange(int lowerInt, int upperInt)
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

double ToggleWriter::p_t(int value)
{
  int IPTG = 0;
  return 9.9/(0.25*(value/(0.0496277915632754*IPTG + 1))*(value/(0.0496277915632754*IPTG + 1)) + 1.99);
}


/**************************Main block definistions*****************/

// Write JetC protion for TetR
// accounts for even or uneven thresholds
void ToggleWriter::write_TetR_JetC(std::fstream& Model)
{
  //if the object has even thresholds then do this
  if(this->TetR_spacing == ToggleWriter::distance::even)
  {
    for(int j = 0; j <= ( TetR_max / TetR_epsilon); j++)
    {
      //for the first line
      if (j == 0 )
      {
        Model << " [] TetR=" << (TetR_epsilon * j) << " -> p_t * "
	      << (10 / static_cast<double>(TetR_epsilon)) << " : (TetR'=TetR+"
	      << TetR_epsilon << ");" << std::endl;
      }
      //for last line
      else if (j == TetR_max / TetR_epsilon)
      {
        Model << " [] TetR=" << (TetR_epsilon * j) << " -> kd * TetR / " 
              << TetR_epsilon << " : (TetR'=TetR-" << TetR_epsilon << ");" 
	      << std::endl;
      }
      //for main portion
      else
      {
        Model << " [] TetR=" << (TetR_epsilon * j) << " -> p_t * " 
	      << (10.0 / static_cast<double>(TetR_epsilon)) << " : (TetR'=TetR+" << TetR_epsilon << ") + kd * TetR / "
              << TetR_epsilon << " : (TetR'=TetR-" << TetR_epsilon << ");" << std::endl;	      
      }
    }
  }
  //else the object has uneven thresholds
  else
  {
    for(int j = 0; j < TetR_points.size(); j++)
    {
      //for first line
      if(j == 0)
      {
	int diff = TetR_points.at(j+1) - TetR_points.at(j);

        Model << " [] TetR=" << TetR_points.at(0) << " -> p_t * " 
              << (10.0 / static_cast<double>(diff)) << " : (TetR'=TetR+" << diff << ");" << std::endl;
      }
      //last line
      else if ( j == TetR_points.size() - 1)
      {
        int diff = TetR_points.at(j) - TetR_points.at(j-1);

	Model << " [] TetR=" << TetR_points.at(j) << " -> kd * TetR / "
              << diff << " : (TetR'=TetR-" << diff << ");" << std::endl;
      }
      //main portion
      else
      {
	int diff = TetR_points.at(j+1) - TetR_points.at(j);

        Model << " [] TetR=" << TetR_points.at(j) << " -> p_t * " << (10.0 / static_cast<double>(diff))
              << " : (TetR'=TetR+" << diff << ") + kd * TetR / ";
	
	//redefine diff to be the difference from current to one bellow
	diff = TetR_points.at(j) - TetR_points.at(j-1);

        Model << diff << " : (TetR'=TetR-" << diff << ");" << std::endl;	
      }
    }
  }
}

// Write JetC protion for LacI
// accounts for even or uneven thresholds
void ToggleWriter::write_LacI_JetC(std::fstream& Model)
{
  //if the object has even thresholds then do this
  if(this->LacI_spacing == ToggleWriter::distance::even)
  {
    for(int j = 0; j <= ( LacI_max / LacI_epsilon); j++)
    {
      //for the first line
      if (j == 0 )
      {
        Model << " [] LacI=" << (LacI_epsilon * j) << " -> p_l * "
	      << (10 / static_cast<double>(LacI_epsilon)) << " : (LacI'=LacI+"
	      << LacI_epsilon << ");" << std::endl;
      }
      //for last line
      else if (j == LacI_max / LacI_epsilon)
      {
        Model << " [] LacI=" << (LacI_epsilon * j) << " -> kd * LacI / " 
              << LacI_epsilon << " : (LacI'=LacI-" << LacI_epsilon << ");" 
	      << std::endl;
      }
      //for main portion
      else
      {
        Model << " [] LacI=" << (LacI_epsilon * j) << " -> p_l * " 
	      << (10.0 / static_cast<double>(LacI_epsilon)) << " : (LacI'=LacI+" << LacI_epsilon << ") + kd * LacI / "
              << LacI_epsilon << " : (LacI'=LacI-" << LacI_epsilon << ");" << std::endl;	      
      }
    }
  }
  //else the object has uneven thresholds
  else
  {
    for(int j = 0; j < LacI_points.size(); j++)
    {
      //for first line
      if(j == 0)
      {
	int diff = LacI_points.at(j+1) - LacI_points.at(j);

        Model << " [] LacI=" << LacI_points.at(0) << " -> p_l * " 
              << (10.0 / static_cast<double>(diff)) << " : (LacI'=LacI+" << diff << ");" << std::endl;
      }
      //last line
      else if ( j == LacI_points.size() - 1)
      {
        int diff = LacI_points.at(j) - LacI_points.at(j-1);

	Model << " [] LacI=" << LacI_points.at(j) << " -> kd * LacI / "
              << diff << " : (LacI'=LacI-" << diff << ");" << std::endl;
      }
      //main portion
      else
      {
	int diff = LacI_points.at(j+1) - LacI_points.at(j);

        Model << " [] LacI=" << LacI_points.at(j) << " -> p_l * " << (10.0 / static_cast<double>(diff))
              << " : (LacI'=LacI+" << diff << ") + kd * LacI / ";
	
	//redefine diff to be the difference from current to one bellow
	diff = LacI_points.at(j) - LacI_points.at(j-1);

        Model << diff << " : (LacI'=LacI-" << diff << ");" << std::endl;	
      }
    }
  }
}

// Write TetR module using Ceska method
// also accounts for even or uneven thresholds
void ToggleWriter::write_TetR_Ceska(std::fstream& Model)
{
  //if the object has even thresholds then do this
  if(this->TetR_spacing == ToggleWriter::distance::even)
  {
    for(int j = 0; j <= ( TetR_max / TetR_epsilon); j++)
    {
      //for the first line
      if (j == 0 )
      {
        Model << " [] TetR=" << (TetR_epsilon * j) << " -> ("
	      << repressionRange( j * TetR_epsilon, (j+1) * TetR_epsilon ) << ") : (TetR'=TetR+"
	      << TetR_epsilon << ");" << std::endl;
      }
      //for last line
      else if (j == TetR_max / TetR_epsilon)
      {
        Model << " [] TetR=" << (TetR_epsilon * j) << " -> kd / (" 
              << degredationRange( (j-1) * TetR_epsilon, (j) * TetR_epsilon) << ") : (TetR'=TetR-" 
	      << TetR_epsilon << ");" << std::endl;
      }
      //for main portion
      else
      {
        Model << " [] TetR=" << (TetR_epsilon * j) << " -> (" 
	      << repressionRange( j * TetR_epsilon, (j+1) * TetR_epsilon ) << ") : (TetR'=TetR+" 
	      << TetR_epsilon << ") + kd / (" << degredationRange( (j-1) * TetR_epsilon, (j) * TetR_epsilon)
	      << ") : (TetR'=TetR-" << TetR_epsilon << ");" << std::endl;	      
      }
    }
   
  }
  //else the object has uneven thresholds
  else
  {
    for(int j = 0; j < TetR_points.size(); j++)
    {
      //for first line
      if(j == 0)
      {
	int diff = TetR_points.at(j+1) - TetR_points.at(j);

        Model << " [] TetR=" << TetR_points.at(0) << " -> (" 
              << repressionRange(TetR_points.at(0), TetR_points.at(j+1)) << ") : (TetR'=TetR+" 
	      << diff << ");" << std::endl;
      }
      //last line
      else if ( j == TetR_points.size() - 1)
      {
        int diff = TetR_points.at(j) - TetR_points.at(j-1);

	Model << " [] TetR=" << TetR_points.at(j) << " -> kd / ("
              << degredationRange(TetR_points.at(j-1), TetR_points.at(j)) << ") : (TetR'=TetR-" 
	      << diff << ");" << std::endl;
      }
      //main portion
      else
      {
	int diff = TetR_points.at(j+1) - TetR_points.at(j);

        Model << " [] TetR=" << TetR_points.at(j) << " -> " 
              << repressionRange(TetR_points.at(j), TetR_points.at(j+1))
              << " : (TetR'=TetR+" << diff << ") + kd / (";
	
	//redefine diff to be the difference from current to one bellow
	diff = TetR_points.at(j) - TetR_points.at(j-1);

        Model << degredationRange(TetR_points.at(j-1), TetR_points.at(j)) << ") : (TetR'=TetR-" << diff << ");" << std::endl;	
      }
    }
  }
}

// Write LacI module using Ceska method
// also accounts for even or uneven thresholds
void ToggleWriter::write_LacI_Ceska(std::fstream& Model)
{
  //if the object has even thresholds then do this
  if(this->LacI_spacing == ToggleWriter::distance::even)
  {
    for(int j = 0; j <= ( LacI_max / LacI_epsilon); j++)
    {
      //for the first line
      if (j == 0 )
      {
        Model << " [] LacI=" << (LacI_epsilon * j) << " -> ("
	      << repressionRange( j * LacI_epsilon, (j+1) * LacI_epsilon ) << ") : (LacI'=LacI+"
	      << LacI_epsilon << ");" << std::endl;
      }
      //for last line
      else if (j == LacI_max / LacI_epsilon)
      {
        Model << " [] LacI=" << (LacI_epsilon * j) << " -> kd / (" 
              << degredationRange( (j-1) * LacI_epsilon, (j) * LacI_epsilon) << ") : (LacI'=LacI-" 
	      << LacI_epsilon << ");" << std::endl;
      }
      //for main portion
      else
      {
        Model << " [] LacI=" << (LacI_epsilon * j) << " -> (" 
	      << repressionRange( j * LacI_epsilon, (j+1) * LacI_epsilon ) << ") : (LacI'=LacI+" 
	      << LacI_epsilon << ") + kd / (" << degredationRange( (j-1) * LacI_epsilon, (j) * LacI_epsilon)
	      << ") : (LacI'=LacI-" << LacI_epsilon << ");" << std::endl;	      
      }
    }
   
  }
  //else the object has uneven thresholds
  else
  {
    for(int j = 0; j < LacI_points.size(); j++)
    {
      //for first line
      if(j == 0)
      {
	int diff = LacI_points.at(j+1) - LacI_points.at(j);

        Model << " [] LacI=" << LacI_points.at(0) << " -> (" 
              << repressionRange(LacI_points.at(0), LacI_points.at(j+1)) << ") : (LacI'=LacI+" 
	      << diff << ");" << std::endl;
      }
      //last line
      else if ( j == LacI_points.size() - 1)
      {
        int diff = LacI_points.at(j) - LacI_points.at(j-1);

	Model << " [] LacI=" << LacI_points.at(j) << " -> kd / ("
              << degredationRange(LacI_points.at(j-1), LacI_points.at(j)) << ") : (LacI'=LacI-" 
	      << diff << ");" << std::endl;
      }
      //main portion
      else
      {
	int diff = LacI_points.at(j+1) - LacI_points.at(j);

        Model << " [] LacI=" << LacI_points.at(j) << " -> " 
              << repressionRange(LacI_points.at(j), LacI_points.at(j+1))
              << " : (LacI'=LacI+" << diff << ") + kd / (";
	
	//redefine diff to be the difference from current to one bellow
	diff = LacI_points.at(j) - LacI_points.at(j-1);

        Model << degredationRange(LacI_points.at(j-1), LacI_points.at(j)) << ") : (LacI'=LacI-" << diff << ");" << std::endl;	
      }
    }
  }
}

/******************End of main block definitions****************/


void ToggleWriter::handleFileName(void)
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
bool ToggleWriter::hasExt(std::string file, std::string ext)
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
