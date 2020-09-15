#include <iostream>
#include <string>
#include "LacI.hpp"


int main()
{

    int IPTG, LacI;
    double threshold;
    std::string name;

    std::cout << "Enter in the amount of IPTG/aTc: ";
    std::cin >> IPTG;
    std::cout << "Enter the final amount of LacI/TetR to be recorded to: ";
    std::cin >> LacI;
    std::cout << "Enter in the name of the resulting file (include .txt): ";
    std::cin >> name;


    /**When creating a LacIgraph object, enter in the following format************************************************
    ***** <name of object>(fixed value of IPTG, amount we want to record LacI to, name of graph as .txt file)*********/
    LacIgraph first(IPTG, LacI, name);


    /**This function simply prints the maximum change in reaction rate and at what corresponding LacI Value**/
    first.getMax();

    std::cout<< "Enter in the threshold for the critical values: ";
    std::cin >> threshold;


    /***This function creates a seperate txt file with the Critical values and their corresponding reaction rate**
    ************It requires an input of at what change in rr do you want the critical values to be stored********/
    first.getCriticalPt(threshold);

    std::cout << "Finished!" << std::endl; 
    std::cout << "The file " << name << " contains data in the format: # of LacI/TetR, reaction rate, instantaneous cnage in rr" << std::endl;
    std::cout << "The file Critical" << name << " contains only the values where the change in rr is greater than " << threshold << std::endl;

    return 0;
}