#!/bin/python3

import json
import sys

###### Class Module #######
class Module:
  def __init__(self, name, pr, dr):
    self.name = name
    self.productionRate = pr
    self.degradationRate = dr

  def makeModule(self):
    print("module " + self.name) 
    print("endmodule //" + self.name) 


###### Class AbstractModel ######
class AbstractModel:
  def __init__(self, infile, outfile):
    self.infile = infile
    self.outfile = outfile
    self.modules = []

  def makeModel(self):
    for x in self.modules:
      x.makeModule() 
      print()

#################################
#########    Main    ############
#################################

n = len(sys.argv)

if n != 2:
  print("Please provide file to be parsed")
  quit()

filename = sys.argv[1]

myAbstract = AbstractModel(filename, "somefile.prism")

with open(filename, 'r') as janiFile:
  print("Parsing file: ", filename)
  janiData = json.load(janiFile)

  # 1st grab the various automaton (modules)
  automatons = []

  for x in janiData["system"]["elements"]:
    automatons.append(x["automaton"])

  # 2nd determine format based on presence of reaction_rates module
  
  #DEF MODEL STYLES

  STYLE_TRAD = 1
  STYLE_SBML = 2
  STYLE = STYLE_TRAD

  for x in automatons:
    if x == "reaction_rates":
      STYLE = STYLE_SBML

  # create empty Modules list
  Modules = ()
      
  # 3rd if reaction rates are in their own module then parse synchronizations
  if STYLE == STYLE_SBML:
    print("Parsing SBML Style model")
    
    # Get syncronizations from original model
    syncs = []
    for x in janiData["actions"]:
      syncs.append(x["name"])

    # Use syncs to find out modules reaction rates
    # 1st: find the reaction rate automata

    for automata in janiData["automata"]:
      if "reaction_rates" == automata["name"]:
        rr_mod = automata
    
    # 2nd: Correlate reaction rate to syncs
    reactionRates = {}
    for action in rr_mod["edges"]:
      for sync in syncs:
        if sync == action["action"]:
          reactionRates[sync] = action["destinations"][0]["probability"]["comment"]

    # 3rd: relate reaction rates to species
    for automata in janiData["automata"]:
      if "reaction_rates" != automata["name"]:
        for edge in automata["edges"]:
          for sync in syncs:
            if sync == edge["action"]:
              print(edge["destinations"][0]["assignments"][0]["ref"] + " correlated with "+ sync)

  # else parse reaction rates of individual modules
  else:
    print("Parsing Traditional Style model")
    for x in automatons:
      if x != "reaction_rates":
        myAbstract.modules.append(Module(x)) 

  #### 
    
  #myAbstract.makeModel()

  #print(type(janiData))
  #print(type(janiData["automata"]))
  #print(type(janiData["automata"][0]))
  #print(janiData["automata"][0].keys())


