#!/bin/python3

import json
import sys

n = len(sys.argv)

if n != 2:
  print("Please provide file to be parsed")
  quit()

filename = sys.argv[1]

with open(filename, 'r') as janiFile:
  print("Parsing file: ", filename)
  janiData = json.load(janiFile)
  print(janiData)
