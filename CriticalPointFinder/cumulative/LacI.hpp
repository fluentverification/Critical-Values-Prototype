#ifndef LACI_HPP
#define LACI_HPP

#include <string>

class LacIgraph
{
public:
LacIgraph(int, int, std::string);

void getMax(void)
{
    std::cout << "# The highest change in reaction rate is " << maxRate << " when the amount of LacI is " << maxLacI << std::endl;
}

void getCriticalPt(double);

private:
    int IPTG;
    int LACI;
    std::string graphName;
    int maxLacI;
    double maxRate;

};


#endif