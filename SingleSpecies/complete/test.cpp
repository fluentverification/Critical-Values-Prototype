
//#include <iostream>
#include <string>
#include "SSWriter.hpp"




int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    std::cout << "Correct usage: ./<program> < int-> model method>\n"
    << "enum for model method"
    << "\n 0. old way repression only"
    << "\n 1. new way repression only"
    << "\n 2. old way degredation only"
    << "\n 3. new way degredation only"
    << "\n 4. old way both"
    << "\n 5. new way both"
    << std::endl;
    return -1;
  }


  SSWriter test(std::string("myFile"), static_cast<SSWriter::method>(std::stoi(argv[1])));

  //std::cout << "The file from the test is " << test.fileName << " with a species of " << test.type << std::endl;


  //test.writeFile();

  return 0;
}
