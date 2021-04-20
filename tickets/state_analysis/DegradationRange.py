#!/bin/python3


def deg( level ):
    return .0075 * 50 / (50 - level)


for i in range(15, 31):
    print(deg(i))
