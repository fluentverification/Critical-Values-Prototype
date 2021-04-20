#!/bin/python3


def deg( level ):
    return .0075 * 50 / (50 - level)

def degrange(low, high):
    return .0075 * high / (high - low)


#for i in range(31, 50):
#    print(degrange(i, 50))

print(degrange(30, 50))
