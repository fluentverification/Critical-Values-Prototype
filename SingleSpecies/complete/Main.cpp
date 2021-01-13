#include <string>
#include <vector>
#include "SSWriter.hpp"

bool is_number(const std::string &name);

void printUsage();

bool runChecks(int argc, char *argv[]);


int main(int argc, char *argv[] )
{
  if(!runChecks(argc, argv)) return -1;

  SSWriter M(std::string(argv[1]), static_cast<SSWriter::method>(std::stoi(argv[2])));

  if(std::string(argv[3]) == "-s")
  {
    M.max = std::stoi(argv[4]);
    M.epsilon = std::stoi(argv[5]);
    M.spacing = SSWriter::distance::even;

  }
  else if(std::string(argv[3]) == "-v")
  {
    std::vector<int> pt;

    for(int i = (argc -1); i > 3; i--)
    {
      pt.push_back(std::stoi(argv[i]));
    }

    M.points = pt;
    M.spacing = SSWriter::distance::uneven;

  }

  M.writeFile();

  return 0;
}



void printUsage()
{
  std::cout << "Correct usage: ./<program> <name> <enum::method> <spacing> [<values> : <total value> <epsilon>]\n"
  << "enum for model method:"
  << "\n 0. old way repression only"
  << "\n 1. new way repression only"
  << "\n 2. old way degredation only"
  << "\n 3. new way degredation only"
  << "\n 4. old way both"
  << "\n 5. new way both" << std::endl;

  std::cout << "spacing: \n-s for standard spacing\n-v for uneven spacing" << std::endl;
}

bool runChecks(int argc, char *argv[])
{

  if (argc < 5)
  {
    printUsage();

    return false;
  }

  if (std::string(argv[3]) != "-s" && std::string(argv[3]) != "-v")
  {
    printUsage();

    return false;
  }

  if (std::string(argv[3]) == "-s" && argc != 6)
  {

    std::cout << "green" << std::endl;
    printUsage();

    return false;
  } 

  if (!is_number(argv[2]))
  {
    std::cout << "Third argument must be an integer refering to the enumeration" << std::endl;
    
    printUsage();

    return false;
  }

  if (std::string(argv[3]) == "-s" && (std::stoi(argv[4]) % std::stoi(argv[5]) != 0))
  {

    std::cout << "If using even threshold spacing, then Epsilon must be a factor of the max value" << std::endl;

    printUsage();

    return false;
  }


  return true;
}


bool is_number(const std::string& s)
{
  return !s.empty() && std::find_if(s.begin(),
      s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}
