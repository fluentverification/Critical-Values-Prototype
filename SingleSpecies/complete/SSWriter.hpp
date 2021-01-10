#include <iostream>
#include <fstream>
#include <sys/stat.h>



#ifndef sswriter_hpp
#define sswriter_hpp

class SSWriter{
public:
  /**
   * enum for our four different methods. 
   * that is:
   * 0. old way repression only
   * 1. new way repression only
   * 2. old way degredation only
   * 3. new way degredation only
   * 4. old way both
   * 5. new way both
   */
  enum method { ORO, NRO, ODO, NDO, OB, NB };
  
  /**
   * enum that tells us the spacing of threshold values
   * 0. evenly spaced
   * 1. uneven
   * defaults to even
   */
  enum distance { even, uneven };


  //public data members
  std::string fileName;
  //std::string species;
  enum method type;
  enum distance spacing;

  int max;
  int thresholds;

  //ctor
  SSWriter(std::string file, enum method type)
            : fileName(file)
            //, species(spec)
	    , max(100)
            , type(type)
	    , spacing(SSWriter::distance::even)
  {writeFile();}

  SSWriter(std::string file)
	    : SSWriter(file, SSWriter::method::ORO)
  { }

  

  //public methods 

  /**
   * Write the file completely
   */
  void writeFile();


  /**
   * Write the header portion
   */
  void writeHeader(std::fstream& Model);


  /**
   * Write the End
   */
  void writeEnd(std::fstream& Model);


  /**
   * write main block with evenly spaced values
   * Becasue there are 6 different situations here
   * it will be easier to determine which to use inside
   * its own function
   */
  void writeMainEven(std::fstream& Model);

  /**
   * write main block with unevenly spaced values
   */
  void writeMainUneven(std::fstream& Model);

  /**
   *
   */
private:
  double repressionRange(int lowerInt, int upperInt);

  double p_t(int value);
};

inline static bool exists (const std::string& filename) {
  struct stat buffer;   
  return (stat (filename.c_str(), &buffer) == 0); 
}

#endif
