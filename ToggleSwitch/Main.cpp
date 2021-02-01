#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
//#include "ToggleSwitch.hpp"

bool is_number(const std::string &name);

void printUsage();

bool runChecks(int argc, char *argv[]);

bool checkSubFlags(std::vector<std::string> subFlags);

int main(int argc, char *argv[] )
{
  if(!runChecks(argc, argv)) return -1;


  return 0;
}



void printUsage()
{
  std::cout << "Correct usage: ./<program> <name> [options for TetR] [options for LacI]\n"
  << "\n"
  << "options for each species: \n"
  << "[--species <initial value> <method> <spacing> {<values> : <max value> <epsilon>}]\n"
  << "\n"
  << "example usage:\n"
  << "./ToggleSwitchWriter test.prism --LacI 26 newWay uneven 0 7 18 26 38 50 --TetR 10 oldWay even 100 5\n";

}

bool runChecks(int argc, char *argv[])
{

  //check for -h or --help flag
  for(int i = 0; i < argc; i++)
  {
    if(std::string(argv[i]) == "-h" || std::string(argv[i]) == "--help")
    {
      printUsage();
      return false;
    }
  }

  //check for both --LacI and --TetR flags
  bool TetR = false;
  bool LacI = false;
  
  for(int i = 0; i < argc; i++)
  {
    if(std::string(argv[i]) == "--LacI")
    {
      LacI = true;
    }
    if(std::string(argv[i]) == "--TetR")
    {
      TetR = true;
    }
  }

  if(!TetR || !LacI)
  {
    printUsage();
    return false;
  }

  
  
  //Check each species specifications
  std::vector<std::string> subFlags;
  std::vector<std::string> subFlags2;

  for(int i = 0; i < argc; i++)
  {
    //std::cout << argv[i] << std::endl;

    if(std::string(argv[i]) == "--LacI")
    {

      do
      {
        subFlags.insert (std::string(argv[i]));
        i++;
      }
      while(std::string(argv[i]) != "--TetR" && i < argc);

    }

    if(std::string(argv[i]) == "--TetR")
    {


      do
      {
        subFlags2.insert (std::string(argv[i]));
        i++;
      }
      while(std::string(argv[i]) != "--LacI" && i < argc);


    }

  }
      checkSubFlags(subFlags);
      checkSubFlags(subFlags2);

  return true;
}

bool checkSubFlags(std::vector<std::string> subFlags)
{
  for(int i = 0; i < subFlags.size(); i++)
  {
    std::cout << subFlags.at(i) << std::endl;
  }
  return true;
}

bool is_number(const std::string& s)
{
  return !s.empty() && std::find_if(s.begin(),
      s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}
