#!/bin/python3


def degRange( lowerInt, upperInt ):
    lower = lowerInt
    upper = upperInt
    j = 0
    for i in range(lower, upper):
        j += (1 / (i + 1))
    return j


for i in range(15, 31):
    print( degRange(0, i) )
