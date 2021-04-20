#!/bin/python3


def p_t( level ):
    return 0.99/(0.25*(level)*(level) + 1.99)

def repRange(low, high):
    return p_t(low) * ( 10 / (high - low) )


#for i in range(51, 100):
#    print(repRange(i, 100))

print(repRange(50, 100))
