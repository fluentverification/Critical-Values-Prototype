#include <algorithm>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <vector>



#ifndef TOGGLEWRITER_HPP
#define TOGGLEWRITER_HPP

class ToggleWriter{
public:
  /**
   * enum for method
   * that is:
   * 0. JetC Method
   * 1. Ceska Method
   */
  enum method { JetC, Ceska };
  
  /**
   * enum that tells us the spacing of threshold values
   * 0. evenly spaced
   * 1. uneven
   * defaults to even
   */
  enum distance { even, uneven };


  //public data members
  std::string fileName;

  //Species Specific Members
  //TetR
  enum method TetR_type;
  enum distance TetR_spacing;
  int TetR_max;
  int TetR_thresholds;

  //if the spacing is even the epsilon will be used
  //otherwise the spacing is uneven and the vector will be
  //used
  int TetR_epsilon;
  std::vector<int> TetR_points;
  
  //LacI
  enum method LacI_type;
  enum distance LacI_spacing;
  int LacI_max;
  int LacI_thresholds;
  int LacI_epsilon;
  std::vector<int> LacI_points;



//  /* Ctors */
//  //ctor for even thresholds??? maybe
//  ToggleWriter(std::string file, enum method type)
//            : fileName(file)
//            , type(type)
//  { }

  //general ctor, I dont think this is used
  ToggleWriter(std::string file)
	    : fileName(file)
  { }

//  //Ctor for uneven thresholds, vector passed in.
//  //This is obselete, the object is created using an above method
//  //and then manually initiallized in Main.cpp
//  ToggleWriter(std::string file, enum method type, std::vector<int> pt)
//	    : points(pt)
//	    , type(type)
//	    , fileName(file)
//	    , spacing(ToggleWriter::distance::uneven)
//  { 
//    max = *std::max_element(pt.begin(), pt.end());
//    std::cout << "Max value is " << max << "\n";
//  }


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
//  void writeEnd(std::fstream& Model);


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


  // Write one species in JetC way
  void write_TetR_JetC(std::fstream& Model);
  void write_LacI_JetC(std::fstream& Model);

  // Write one species in Ceska way
  void write_TetR_Ceska(std::fstream& Model);
  void write_LacI_Ceska(std::fstream& Model);

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
