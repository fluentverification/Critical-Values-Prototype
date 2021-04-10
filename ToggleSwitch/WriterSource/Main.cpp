#include <string>
#include <vector>
#include "ToggleWriter.hpp"

bool is_number(const std::string &name);

//void printUsage();

//bool runChecks(int argc, char *argv[]);

/*
int main(int argc, char *argv[] )
{

  int arg_i = 4;

  std::cout << "Argv[3] TetR method is " << argv[3] << std::endl;

  std::cout << "Argv[2] TetR spacing is " << argv[2] << std::endl;

  if(std::string(argv[2]) == "-s")
  {
    std::cout << " TetR max is standardly spaced" << std::endl;
    std::cout << " TetR max is " << argv[4] << std::endl;
    std::cout << " TetR epsilon is " << argv[5] << std::endl;
    ++arg_i;
    ++arg_i;
  }
  else if(std::string(argv[2]) == "-v")
  {
    std::cout << " TetR max is unevenly spaced" << std::endl;

    //std::vector<int> pt;

    for(; is_number(argv[arg_i]) ; arg_i++)
    {
      std::cout << " Pushing back value " << argv[arg_i] << std::endl;
      //pt.push_back(std::stoi(argv[arg_i]));
    }

    //M.TetR_points = pt;
    //M.TetR_spacing = ToggleWriter::distance::uneven;
    //M.TetR_max = *std::max_element(pt.begin(), pt.end());
  }

  std::cout << "Argv[" << arg_i + 1 << "] LacI method is " << argv[arg_i + 1] << std::endl;
  std::cout << "Argv[" << arg_i << "] LacI spacing is " << argv[arg_i] << std::endl;

  if(std::string(argv[arg_i]) == "-s")
  {
    ++arg_i;
    ++arg_i;
    std::cout << "Argv[" << arg_i << "] LacI max is " << argv[arg_i] << std::endl;
    ++arg_i;
    std::cout << "Argv[" << arg_i << "] LacI epsilon is " << argv[arg_i] << std::endl;
    ++arg_i;

  }
  else if(std::string(argv[arg_i]) == "-v")
  {
    arg_i++;
    ++arg_i;

    for(; arg_i < argc ; arg_i++)
    {
      std::cout << "Push back to LacI vector" << argv[arg_i] << std::endl;
    }

    //M.LacI_points = pt;
    //M.LacI_spacing = ToggleWriter::distance::uneven;
    //M.LacI_max = *std::max_element(pt.begin(), pt.end());

  }

  return 0;
}

*/

int main(int argc, char *argv[] )
{
  //if(!runChecks(argc, argv)) return -1;

  int arg_i = 4;

  ToggleWriter M = ToggleWriter(std::string(argv[1]));

  M.TetR_type = static_cast<ToggleWriter::method>(std::stoi(argv[3]));
  
  if(std::string(argv[2]) == "-s")
  {
    M.TetR_max = std::stoi(argv[4]);
    M.TetR_epsilon = std::stoi(argv[5]);
    M.TetR_spacing = ToggleWriter::distance::even;
    ++arg_i;
    ++arg_i;

  }
  else if(std::string(argv[2]) == "-v")
  {
    std::vector<int> pt;

    for(; is_number(argv[arg_i]) ; arg_i++)
    {
      pt.push_back(std::stoi(argv[arg_i]));
    }

    M.TetR_points = pt;
    M.TetR_spacing = ToggleWriter::distance::uneven;
    M.TetR_max = *std::max_element(pt.begin(), pt.end());
  }
  
  M.LacI_type = static_cast<ToggleWriter::method>(std::stoi(argv[arg_i + 1]));

  if(std::string(argv[arg_i]) == "-s")
  {

    ++arg_i;
    ++arg_i;
    M.LacI_max = std::stoi(argv[ arg_i ]);
    ++arg_i;
    M.LacI_epsilon = std::stoi(argv[ arg_i ]);
    M.LacI_spacing = ToggleWriter::distance::even;

  }
  else if(std::string(argv[arg_i]) == "-v")
  {
    std::vector<int> pt;
    
    ++arg_i;
    ++arg_i;

    for(; arg_i < argc ; arg_i++)
    {
      pt.push_back(std::stoi(argv[arg_i]));
    }

    M.LacI_points = pt;
    M.LacI_spacing = ToggleWriter::distance::uneven;
    M.LacI_max = *std::max_element(pt.begin(), pt.end());

  }
  

  M.writeFile();

  return 0;
}


//
//void printUsage()
//{
//  std::cout << "Correct usage: ./<program> <name> [Options for TetR] [Options for LacI]\n;
//  std::cout << "Options for Species: <spacing> <method> [<values> : <total value> <epsilon>]\n"
//  << "enum for model method:"
//  << "\n 0. Jetc Method"
//  << "\n 1. Ceska Method" << std::endl;
//
//  std::cout << "spacing: \n-s for standard spacing\n-v for uneven spacing" << std::endl;
//}
//
//bool runChecks(int argc, char *argv[])
//{
//
//  if (argc < 5)
//  {
//    printUsage();
//
//    return false;
//  }
//
//  if (std::string(argv[3]) != "-s" && std::string(argv[3]) != "-v")
//  {
//    printUsage();
//
//    return false;
//  }
//
//  if (std::string(argv[3]) == "-s" && argc != 6)
//  {
//
//    std::cout << "green" << std::endl;
//    printUsage();
//
//    return false;
//  } 
//
//  if (!is_number(argv[2]))
//  {
//    std::cout << "Third argument must be an integer refering to the enumeration" << std::endl;
//    
//    printUsage();
//
//    return false;
//  }
//
//  if (std::string(argv[3]) == "-s" && (std::stoi(argv[4]) % std::stoi(argv[5]) != 0))
//  {
//
//    std::cout << "If using even threshold spacing, then Epsilon must be a factor of the max value" << std::endl;
//
//    printUsage();
//
//    return false;
//  }
//
//
//  return true;
//}


bool is_number(const std::string& s)
{
  return !s.empty() && std::find_if(s.begin(),
      s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}
