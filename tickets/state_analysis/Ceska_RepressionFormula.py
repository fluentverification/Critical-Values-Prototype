#!/bin/python3

def p_t( level ):
    return 9.9/(0.25*(level)*(level) + 1.99)


def repRange( lowerInt, upperInt ):
    lower = lowerInt
    upper = upperInt
    j = 0
    for i in range(lower, upper):
        j += (1/p_t(i))
    return 1/j


for i in range(15, 31):
    print( repRange(i, 50) )
