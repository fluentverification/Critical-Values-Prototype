#include <algorithm>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <vector>



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
  enum method type;
  enum distance spacing;

  int max;
  int thresholds;

  //if the spacing is even the epsilon will be used
  //otherwise the spacing is uneven and the vector will be
  //used
  int epsilon;
  std::vector<int> points;


  /* Ctors */
  //ctor for even thresholds??? maybe
  SSWriter(std::string file, enum method type)
            : fileName(file)
            , type(type)
  { }

  //general ctor, I dont think this is used
  SSWriter(std::string file)
	    : fileName(file)
  { }

  //Ctor for uneven thresholds, vector passed in.
  //This is obselete, the object is created using an above method
  //and then manually initiallized in Main.cpp
  SSWriter(std::string file, enum method type, std::vector<int> pt)
	    : points(pt)
	    , type(type)
	    , fileName(file)
	    , spacing(SSWriter::distance::uneven)
  { 
    max = *std::max_element(pt.begin(), pt.end());
    std::cout << "Max value is " << max << "\n";
  }


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
  void writeMain(std::fstream& Model);

  /**
   * write main block with unevenly spaced values
   */
  //void writeMainUneven(std::fstream& Model);

private:

/************************************************************/
  /**
   * Because there are so many different main block writers
   * I will place the declarations in their own category
   */

  // Write main portion old way repression only
  void writeORO(std::fstream& Model);
  
  // Write new way repressino only
  void writeNRO(std::fstream& Model);

  // Write old way degredation only
  void writeODO(std::fstream& Model);

  // Write new way degredation only
  void writeNDO(std::fstream& Model);

  // Write Both in the old way
  void writeOB(std::fstream& Model);

  // Write Both in the new way
  void writeNB(std::fstream& Model);

/************************************************************/

  /**
   * Handles the file name, 
   * First checks to see if the file has the correct extension
   * if not it adds it
   * Then it checks to see if it already exists, if it does it 
   * deletes it
   */
  void handleFileName();

  /**
   * static funtions that checks if a given extension is 
   * on the also given string
   */
  static bool hasExt(std::string file, std::string ext);

  double repressionRange(int lowerInt, int upperInt);

  double degredationRange(int lowerInt, int upperInt);

  double p_t(int value);
};

inline static bool exists (const std::string& filename) {
  struct stat buffer;   
  return (stat (filename.c_str(), &buffer) == 0); 
}

#endif
