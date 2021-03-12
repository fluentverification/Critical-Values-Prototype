#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include "LacI.hpp"

using namespace std;

LacIgraph::LacIgraph(int IPTG, int LACI, std::string graphName)
{
    this->IPTG = IPTG;
    this->LACI = LACI;
    this->graphName = graphName;


    ofstream myplot;
    myplot.open(graphName);

    double LacI;
    
    maxLacI = 0;
    maxRate = 0;

    double kd, Kc, nc, Kr, Ko, kp, np, RNAP, Ptet, Plac;
    kd = .0075;
    Kc = .05;
    nc = 2;
    Kr = .5;
    Ko = .033;
    kp = .05;
    np = 10;
    RNAP = 30;
    Ptet = 2;
    Plac = 2;

    double top, bottom, sub1, sub2, final;

    LacI = 0;



    for (int i = 0; i < LACI ; i++, LacI++)
    {

    top = nc * kp * Plac * Ko * RNAP;
    bottom = 1 + (Ko * RNAP) + pow(( (Kr * LacI)/(1 + (Kc * IPTG))    ),nc);
    final = top / bottom;

    myplot.width(8);                    //Formats the output file
    myplot << left << i << ' ';         //as to make it easier 
    myplot.width(16);                   //to view
    myplot << final << ' ';


/***************writes the derivative to the function to the file*************/
    top = nc * kp * Plac * Ko * RNAP;
    bottom = 1 + (Ko * RNAP) + pow(( (Kr * LacI)/(1 + (Kc * IPTG))    ),nc);
    sub1 = Kr * nc * pow( LacI, (nc - 1));
    sub2 = pow((1 + Kc * IPTG), nc);
    final = (top * sub1) / (pow((bottom), 2) * sub2);

    myplot << -final << endl;

/**Finds the Maximum change in reaction rate and at what Lacl value that occurs**/
        if (final > maxRate)
            {
                maxRate = final;
                maxLacI = i;
            }


    }


    myplot.close();
}


/***************************************************************
This function gives the critical points by comparing the 
previous data point with the next data point and records
a point if the change in reaction rate between the next point
and the current point is greater than a predertermined amount...
(or potentially a fraction of the Maximum change in reaction rate)
***************************************************************/
void LacIgraph::getCriticalPt(double limit)
{
    fstream file;
    ofstream Critical;

    file.open(graphName, fstream::in);
    Critical.open("Critical" + graphName);

    int prev, current;
    double CinRR, prevRate, currentRate;

    prevRate = 0;
    prev = 0;

    while(!file.eof())
    {
        file >> current >> currentRate >> CinRR;

        if(abs(currentRate - prevRate) > (limit))
        {
            Critical << current << " " << currentRate << endl;
            prev = current;
            prevRate = currentRate;
        }

        

    }

    file.close();
    Critical.close();
}