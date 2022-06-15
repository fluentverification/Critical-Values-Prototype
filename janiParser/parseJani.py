#!/bin/python3

import json
import sys
import subprocess

DEBUG = False

###### Utils #######
def runEQ(eq):
  call = "echo \"" + eq + "\" | bc -l"
  val = subprocess.check_output(call, shell=True).decode('utf-8').strip().split('\n')
  val = float(val[0])
  return val

def debug(somestring):
  global DEBUG
  if DEBUG == True:
    print(somestring)

def isDebug():
  global DEBUG
  return DEBUG

###### Reaction Rate class #####
class ReactionRate:
  def __init__(self, rate, sync, val):
    self.rate = rate
    self.sync = sync
    self.val = val

###### Class Module #######
class Module:
  def __init__(self, name, var):
    self.name = name
    self.var = var
    self.productionRates = []  # Production rate for 1 molecule
    self.degradationRates = [] # degradation rate for 1 modecule
    self.states = set() 
    self.allReactions = []

  def makeModule(self):
    print("module " + self.name + '\n') 
    print("  " + self.name + " : [" + str(self.var.minimum) + ".." 
          + str(self.var.maximum) + "] init " + str(self.var.initial) + ";\n")

    # for each production state:
    statelist = list(self.states) 
    statelist.sort()
    curr = 0
    prev = 0
    for nextval in statelist:
      if nextval == 0:
        continue
      else:
        for prod in self.productionRates:
          print("  [" + prod.sync + "_" + str(curr) + "] " + self.name + " = " + str(curr) + " -> (" + self.name + "\'=" + str(nextval) + ");" )
          rr = str("  [" + prod.sync + "_" + str(curr) + "] (" + prod.rate + "*" + str(prod.val) + ") / " + str(nextval -  curr) + " > 0 : true;")
          self.allReactions.append(rr)
        for deg in self.degradationRates:
          print("  [" + deg.sync + "_" + str(curr) + "] " + self.name + " = " + str(curr) + " -> (" + self.name + "\'=" + str(prev) + ");" )
          rr = str("  [" + deg.sync + "_" + str(curr) + "] (" + deg.rate + "*" + str(deg.val) + ") / " + str(curr - prev) + " > 0 : true;")
          self.allReactions.append(rr)
      
      prev = curr
      curr = nextval

      
    print("\nendmodule // " + self.name) 

###### Class Variable #######
# The purpose of this class is to behave as a struct to hold
# information of each variable or species
class Variable:
  def __init__(self, var, minimum, maximum, init, needed):
    self.variable = var
    self.minimum = minimum
    self.maximum = maximum
    self.initial = init
    # This means necessary to keep a specific value for this variable
    # during the __fillVars step
    # for necessary==true it means this variable will be replaced with a constant
    # value during threshold generation because this variable doesn't have any transitions
    self.necessary = needed 

###### Class AbstractModel ######
class AbstractModel:
  def __init__(self, infile, outfile):
    self.infile = infile
    self.outfile = outfile
    self.modules = dict() 
    self.variables = dict()

  def makeModel(self):
    for x in self.modules:
      self.modules[x].makeModule()
      print()
    self.__makeRRModule()

  def printModules(self):
    for x in self.modules:
      print(x)

  def printModuleStates(self):
    print("Printing module states:")
    for x in self.modules:
      print("For module " + x + ":")
      for state in self.modules[x].states:
        print(state)

  def __makeRRModule(self):
    print("module reaction_rates\n")
    for x in self.modules:
      print("  // module: " + x)
      for y in self.modules[x].allReactions:
        print(y)
      print()

    print("endmodule //reaction_rates\n")

  def __fillVars(self, eq):
    result = eq
    for x in self.variables: 
      var = self.variables[x] 
      if(result.find(var.variable) != -1 ):
        if(var.necessary):
          result = result.replace(var.variable, str(var.initial))
    return result

  def __determineThresholds(self, var, eq, tol):
    # 1st: Determine maximum rate
    maxchange = 0.0
    for current in range(var.minimum, var.maximum + 1):
      temp = eq.replace(var.variable, str(current))
      maxchange = max(maxchange, runEQ(temp))
    debug("Max change for " + var.variable + " is " + str(maxchange))

    # 2nd: Use maxchange and tol to determine distance between two states
    debug("Thresholds for " + var.variable + ":")
    delta = tol * maxchange 
    self.modules[var.variable].states.add(var.minimum)
    self.modules[var.variable].states.add(var.maximum)
    self.modules[var.variable].states.add(var.initial)

    temp = eq.replace(var.variable, str(var.initial))
    prevrate = runEQ(temp)
    runningsum = 0.0

    for current in range(var.minimum, var.maximum + 1):
      temp = eq.replace(var.variable, str(current))
      rate = runEQ(temp)
      tempsum = abs(prevrate - rate) + runningsum

      if tempsum > delta:
        #debug("\t" + str(current))
        self.modules[var.variable].states.add(current)
        prevrate = rate
        runningsum = 0.0
      else:
        runningsum = tempsum

  # This function takes in a tolerance and determines
  # the Thresholds for each module
  def makeAbstract(self, tol):
    for module in self.modules:
      for rate in self.modules[module].productionRates:
        # replace static variables
        fixed = self.__fillVars(rate.rate)
        # vary leftover variables 
        # this is where the thresholds are determined
        # TODO: this only accounts for equations with one variable
        # make it account for the possibility for two or more
        for x in self.variables: 
          var = self.variables[x] 
          if(fixed.find(var.variable) != -1):
            self.__determineThresholds(var, fixed, tol)
            #print(var.variable + " ::: " + fixed)
    
    # now we have the thresholds, we need to 

#################################
#########    Main    ############
#################################

n = len(sys.argv)

if n != 3:
  print("Please provide file to be parsed and a tolerance")
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
      
  # 3rd if reaction rates are in their own module then parse synchronizations
  if STYLE == STYLE_SBML:
    print("Parsing Syncronized model")
    
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
    # TODO: consider mapping syncs here instead of 
    #       grabbing syncs above. One step instead of two
    for action in rr_mod["edges"]:
      for sync in syncs:
        if sync == action["action"]:
          #reactionRates[sync] = action["destinations"][0]["probability"]["comment"]
          reactionRates[sync] = action["rate"]["comment"]
          #print(type(action["rate"]["exp"]))
          #left = action["rate"]["exp"]
          #while type(left) == dict:
          #  print(type(left))
          #  left = left["left"]

          #print(left)
          # TODO: instead of grabbing comment, lets grab the whole tree.

    
    allvars = dict()
    for var in janiData["variables"]:
      if var["type"] != "real":
        init = var["initial-value"]
        maxi = var["type"]["upper-bound"]
        mini = var["type"]["lower-bound"]
        allvars[var["name"]] = Variable(var["name"], mini, maxi, init, True)
      #allmods[automata["name"]] = True

    # 3rd: relate reaction rates to species
    for automata in janiData["automata"]: # for all "modules"
      if "reaction_rates" != automata["name"]: # if the module isn't the reaction rate module
        for edge in automata["edges"]: # for each edge
          for sync in syncs: # for each sync
          # TODO: instead of iterating every sync, use the current edges sync
          #       to directly access the reaction rate
            if sync == edge["action"]: # if the sync corresponds to the edge
              modname = edge["destinations"][0]["assignments"][0]["ref"]
              if not modname in myAbstract.modules:
                newmod = Module(edge["destinations"][0]["assignments"][0]["ref"], allvars[modname])
                myAbstract.modules[modname] = newmod
                # if the variable has a corresponding reaction rate to some
                # sort of repression/degradation, then it is marked not necessary 
                allvars[modname].necessary = False 

              mod = myAbstract.modules[modname]
              amount = edge["destinations"][0]["assignments"][0]["value"]["right"]
              rr = ReactionRate(reactionRates[sync], sync, amount)
              if edge["destinations"][0]["assignments"][0]["value"]["op"] == "+":
                mod.productionRates.append(rr)
                #mod.productionRates.append(reactionRates[sync] + " / " + str(amount) )
              else:
                mod.degradationRates.append(rr)

    myAbstract.variables = allvars

    #for var in allvars:
    #  if allvars[var].necessary:
    #    print(var + " is a variable")
    #  else:
    #    print(var + " is a module")

    ## 4th: grab everything else that's needed
    #for automata in janiData["automata"]:
    #  if automata is not in myAbstract.modules  
    #    mark it as a const variable

  # else parse reaction rates of individual modules
  else:
    print("Parsing Traditional Style model")
    for x in automatons:
      if x != "reaction_rates":
        myAbstract.modules.append(Module(x)) 

  #### make abstractions ####
    
  myAbstract.makeAbstract(float(sys.argv[2]))
  #myAbstract.printModules()
  #myAbstract.printModuleStates()
  myAbstract.makeModel()

