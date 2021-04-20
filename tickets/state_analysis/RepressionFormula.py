#!/bin/python3


def p_t( level ):
    return 0.99/(0.25*(level)*(level) + 1.99)


for i in range(15, 31):
    print(p_t(i) * ( 10 / (50 - i) ))
