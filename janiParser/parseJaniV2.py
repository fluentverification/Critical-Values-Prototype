#!/bin/python3
import argparse
import json
import sys
import subprocess
import multiprocessing.dummy as mp

##############################
###### Global variables ######
##############################

DEBUG = False
CORES = 4

STYLE_TRAD = 1
STYLE_SBML = 2

#######################
###### Utilities ######
#######################

###### Utility: printUsage ######
# The purpose of this utility is self explanatory
def printUsage():
  print("Usage: " + sys.argv[0] + " <filename> <tolerance>")

###### Utility: runEQ ######
# The purpose of this utility is to allow for 
# a string to be evaluated as an equation
#
# TODO: this process is what causes this program to take so long to evaluate.
#       If anywhere, this is the best place to optimize
#
# eq: the equation to be evaluated
def runEQ(eq):
  call = "echo \"" + eq + "\" | bc -l"
  val = subprocess.check_output(call, shell=True).decode('utf-8').strip().split('\n')
  val = float(val[0])
  return val

###### Utility: isDebug ######
# The purpose of this utility is simply to
# return true if in debug mode. It uses the 
# global variable DEBUG to check
def isDebug():
  global DEBUG
  return DEBUG

###### Utility: debug ######
# The purpose of this utility is to print
# information given the global variable
# DEBUG is true
#
# msg: the message to be printed if in debug
def debug(msg):
  if isDebug():
    print(msg)

#####################
###### Classes ######
#####################

###### Class: ReactionRate ######
# The purpose of this class is to hold information on a 
# given reaction rate
class ReactionRate:
  ### Constructor ###
  # rate: the reaction rate string itself
  # sync: the cooresponding sync parsed from the model
  # val: the number of molecules for which the rate represents
  def __init__(self, rate, sync, val):
    self.rate = rate
    self.sync = sync
    self.value = val

###### Class: Variable ######
# The purpose of this class is to hold information regarding
# a Variable. Information such as variable name, minimum value
# maximum value, initial value, and static behavior are held.
class Variable:
  ### Constructor ###
  # name: the variable name
  # minimum: minimum value
  # maximum: maximum value
  # init: initial value
  # static: Does the variable change
  def __init__(self, name, minimum, maximum, init, static=False):
    self.name = name
    self.minimum = minimum
    self.maximum = maximum
    self.initial = init
    self.static = static

###### Class: Module ######
class Module:
  ### Constructor ###
  # Upon initialization, the only thing that may be known
  # is the name of the variable
  #
  # var: the Variable (as in above defined type) the module uses
  def __init__(self, var):
    self.var = var
    # Here we will declare other local variables as well 
    self.productionRates = [] # set of ReactionRate functions that represent
                              # all repression rates for this variable
    self.degradationRates = [] # same as above but for degradation rates
    self.states = set() # the states to be made in the final model
    self.manualStates = set() # the states that might have been added manually
    self.returnedReactions = [] # as thresholds are generated, states are 
                                # needed to be added to the ReactionRate  
                                # module, so strings are collected here that 
                                # will simply be printed later
  
  ### addStates ###
  # Allows for the manual addition of states.
  # If a certain state is desired to be added, then this is the funciton to 
  # add it. If states are added into manualStates, then the model will only 
  # use those states, so the override will not add them to manualStates if 
  # you desire to union the generated and manual states.
  # Also note, later, initial, min, and maximum values are always turned into
  # states regardless
  #
  # someset: any container of values that can be iterated over
  # override: a boolean that determines if the states will override
  #           generated values
  def addStates(self, someset, override=True):
    if override:
      for x in someset:
        self.manualStates.add(x)
    else:
      for x in someset:
        self.states.add(x)

  ### makeModule ###
  # This function prints the module and adds strings to returnedReactions
  # that will be printed out after all modules.
  # This is where the module is actually printed out.
  # All states should be added by this point, either manually or generated.
  #
  # outfile: the name of output file
  def makeModule(self, outfile):
    # localize definitions
    name = self.var.name 
    min = self.var.minimum
    max = self.var.maximum
    init = self.var.initial

    print("module " + name + '\n')
    print("  " + name + " : [" + str(min) + ".." + str(max) + "] init " + str(init) + ";")
    print()

    # determine whether or not to use manualStates or not
    if len(self.manualStates) != 0: # if there are manual states
      # erase set
      self.states.clear()
      self.addStates(self.manualStates, False)

    # check if any states have been added
    if len(self.states) == 0:
      print("ERROR: no states have been added to module " + name)
      quit()

    # add init, min, and max as states
    self.addStates([init, min, max], False)

    statelist = list(self.states) 
    statelist.sort()

    curr = 0 # keeps track of current state
    prev = 0 # tracks previous state
    
    for nextval in statelist:
      if nextval == 0: # skips first value to align curr correctly
        continue
      elif curr == 0: # catches case that only production occurs at this level 
        self.__printProduction(curr, nextval)
      else:
        self.__printProduction(curr, nextval)
        self.__printDegradation(prev, curr)

      prev = curr
      curr = nextval

    # print the final line where only degradation occurs
    self.__printDegradation(prev, curr)

    print("\nendmodule // " + name)

  ### __printProduction ###
  # private method to iterate over each production rate
  # and print a line according to the input current and next values
  # 
  # curr: the current value
  # nextval: the next value
  def __printProduction(self, curr, nextval):
    name = self.var.name 
    for prod in self.productionRates:
      print("  [" + prod.sync + "_" + str(curr) + "] " + name + " = " + str(curr)
            + " -> (" + name + "\'=" + str(nextval) + ");" )
      rr = str("  [" + prod.sync + "_" + str(curr) + "] (" + prod.rate + "*" 
               + str(prod.value) + ") / " + str(nextval -  curr) + " > 0 -> " 
               + "("+ prod.rate + "*" + str(prod.value) + ") / " 
               + str(nextval - curr) + " : true;")
      self.returnedReactions.append(rr)

  ### __printDegradation ###
  # private method to iterate over each degradation rate
  # and print a line according to the current and previous value
  # 
  # prev: previous value
  # curr: current value
  def __printDegradation(self, prev, curr):
    name = self.var.name 
    for deg in self.degradationRates:
      print("  [" + deg.sync + "_" + str(curr) + "] " + name + " = " + str(curr)
            + " -> (" + name + "\'=" + str(prev) + ");" )
      rr = str("  [" + deg.sync + "_" + str(curr) + "] (" + deg.rate + "*" 
               + str(deg.value) + ") / " + str(curr - prev) + " > 0 -> " 
               + "("+ deg.rate + "*" + str(deg.value) + ") / " 
               + str(curr - prev) + " : true;")
      self.returnedReactions.append(rr)

###### Class: Abstract Model ######
# The purpose of this class is to manage all the processes needed
# to parse, sort, and abstract the parts from a jani (json) file
# and create an abstract model based off these parts
class AbstractModel:
  ### Constructor ###
  # This constructor first parses the jani file.
  # It then initializes a few member variables
  #
  # infile: the jani file to base the abstraction
  def __init__(self, infile):
    debug("Parsing file: " + infile) 
    with open(infile, 'r') as janiFile: # read in jani file
      self.janiData = json.load(janiFile)

    self.modules = dict() # a dictionary of the various modules (automata)
    self.variables = dict() # a dictionary of the variables
    
    # 1. determine model type
    style = self.__style()
    if style == STYLE_TRAD:
      self.__parseTraditional()
    elif style == STYLE_SBML:
      self.__parseSBML()
    else:
      print("ERROR: undetermined model style")
      quit()

  ### makeAbstract ###
  # The purpose of this method is to iterate over each module and determine
  # thresholds for each
  #
  # TODO: This only accounts for one variable and will probably blow up if
  #       the reaction rate has more than one non-static variable
  # 
  # tol: the tolerance to base abstraction
  #@# Normal method
  def makeAbstract(self, tol):
    #for module in self.modules:
    #  self.__makeAbstractModule(tol, module)
      #for rate in self.modules[module].productionRates:
      #  fixed = self.__fillVars(rate.rate) # replace static variables
      #  for x in self.variables:
      #    var = self.variables[x]
      #    if(fixed.find(var.name) != -1):
      #      self.__determineThresholds(var, fixed, tol)
  #@# Experimental multithreaded way
    global TOLERANCE, CORES
    TOLERANCE = tol
    p = mp.Pool(CORES)
    p.map(self.__makeAbstractModule, self.modules)
    p.close()
    p.join()

  ### addStates ###
  # The purpose of this method is to add states to one of the modules.
  # If the module doesn't exist then an error is thrown. Essentially, 
  # this just wraps the addStates in the Module class.
  #
  # module: the module name in question
  # stateset: the set of states to be added
  # override: do these states override the generated ones?
  def addStates(self, module, stateset, override):
    if module in self.modules:
      self.modules[module].addStates(stateset, override)
    else:
      print("Module " + module + " does not exist in model")
      quit()

  ### makeModel ###
  # The purpose of this method is to initiate the print process.
  # This is called after all states have been added/generated.
  # 
  # outfile: The name of the output prism file 
  def makeModel(self, outfile):
    print("ctmc\n")
    # print constants
    for y in self.variables:
      if self.variables[y].static:
        print("const double " + y + " = " + str(self.variables[y].initial) + ";")

    print()

    # make each module 
    for x in self.modules:
      self.modules[x].makeModule(outfile)
      print()
    # make the reaction rate module last
    print("module reaction_rates\n")
    for x in self.modules:
      print("  // module: " + x)
      for y in self.modules[x].returnedReactions:
        print(y)
      print()
    
    print("endmodule // reaction_rates\n")

  ### __makeAbstractModule ###
  # The purpose of this method is to abstract a single module and determine
  # the reaction rates
  # 
  # module: the module in question
  def __makeAbstractModule(self, module):
    global TOLERANCE
    for rate in self.modules[module].productionRates:
      fixed = self.__fillVars(rate.rate) # replace static variables
      for x in self.variables:
        var = self.variables[x]
        if(fixed.find(var.name) != -1):
          self.__determineThresholds(var, fixed, TOLERANCE)

  ### __determineThresholds ###
  # The purpose of this method is to vary the variable in question in the 
  # equation provided and determine thresholds based off the provided
  # tolerance
  #
  # var: Variable (type) in question
  # eq: The equation in question (should only have one variable) 
  # tol: The tolerance
  def __determineThresholds(self, var, eq, tol):
    # 1st: Determine maximum rate
    maxchange = 0.0 
    for current in range(var.minimum, var.maximum + 1): 
      temp = eq.replace(var.name, str(current))
      maxchange = max(maxchange, runEQ(temp))
    debug("Max change for " + var.name + " is " + str(maxchange))

    # 2nd: Use maxchange and tol to determine distance between two states
    debug("Thresholds for " + var.name + ":")
    delta = tol * maxchange 
    temp = eq.replace(var.name, str(var.initial))
    prevrate = runEQ(temp)
    runningsum = 0.0 

    for current in range(var.minimum, var.maximum + 1): 
      temp = eq.replace(var.name, str(current))
      rate = runEQ(temp)
      tempsum = abs(prevrate - rate) + runningsum

      if tempsum > delta:
        debug("\t" + str(current))
        self.modules[var.name].states.add(current)
        prevrate = rate
        runningsum = 0.0 
      else:
        runningsum = tempsum
  
  ### __fillVars ###
  # The purpose of this method is to fill in any variables that may be static
  # or aren't the variable in question. A self context is needed as variables
  # from self.variables are used to extract the replacement data
  #
  # eq: The rate equation in question
  # return: the fixed up equation
  def __fillVars(self, eq):
    result = eq 
    for x in self.variables:
      var = self.variables[x]
      debug("Attempting to fill varaible of type " + str(type(var)))
      debug("Attempting to fill variable of name " + var.name)
      if(result.find(var.name) != -1 ):
        if(var.static):
          result = result.replace(var.name, str(var.initial))
    return result

  ### __style ###
  # The purpose of this function is to return the 
  # style of the model
  def __style(self):
    for x in self.janiData["system"]["elements"]:
      if x["automaton"] == "reaction_rates":
        debug("Model is Syncronized style")  
        return STYLE_SBML
    debug("Model is Traditional style") 
    return STYLE_TRAD

  ### __parseTraditional ###
  # The purpose of this function is to manage processes for the case 
  # that the model is traditional
  def __parseTraditional(self):
    debug("Parsing Traditional style model")
    print("this style model has not been developed")
    quit()

  ### __parseSBML ###
  # The purpose of this function is to manage process for the parsing
  # an SBML style model
  def __parseSBML(self):
    debug("Parsing Syncronized style model")
    # 1st: find the reaction rate module
    for automata in self.janiData["automata"]:
      if "reaction_rates" == automata["name"]:
        rr_mod = automata
        break
    # 2nd: Correlate reaction rate to syncs
    reactionRates = {}
    for action in rr_mod["edges"]:
      for sync in self.janiData["actions"]:
        if sync["name"] == action["action"]:
          reactionRates[sync["name"]] = action["rate"]["comment"]
    # 3rd: Grab all the variables
    for var in self.janiData["variables"]:
      if var["type"] != "real":
        debug("Adding variable: " + var["name"])
        init = var["initial-value"]
        maxi = var["type"]["upper-bound"]
        mini = var["type"]["lower-bound"]
        self.variables[var["name"]] = Variable(var["name"], mini, maxi, init, True)
        debug("As type: " + str(type(self.variables[var["name"]])))
    # 4th: relate reaction rates to species and create modules
    for automata in self.janiData["automata"]:
      if "reaction_rates" == automata["name"]: # skip reaction rate module
        continue 
      for edge in automata["edges"]:           # for each edge in the automata
        for sync in self.janiData["actions"]:  
          if sync["name"] == edge["action"]:   # correlate sync to edge
            modname = edge["destinations"][0]["assignments"][0]["ref"]
            if not modname in self.modules:    # if the module doesn't already exist
              self.variables[modname].static = False  # set the variable to non-static
              newmod = Module(self.variables[modname])
              self.modules[modname] = newmod
            
            mod = self.modules[modname]
            # determine the amount by which the reaction rate covers
            amount = edge["destinations"][0]["assignments"][0]["value"]["right"] 
            # create the reactionrate
            rr = ReactionRate(reactionRates[sync["name"]], sync["name"], amount)
            # determine if it is a production or degredation reaction
            if edge["destinations"][0]["assignments"][0]["value"]["op"] == "+":
              mod.productionRates.append(rr)
            else:
              mod.degradationRates.append(rr)

##################
###### Main ######
##################

if __name__ == "__main__":
  # Gather number of arguments
  n = len(sys.argv)

  if n != 3:
    printUsage()
    quit()

  myAbstract = AbstractModel(sys.argv[1])
  myAbstract.makeAbstract(float(sys.argv[2]))
  myAbstract.addStates("_C", {0,10,20,30,40,50,60,70,80,90,100,110,120,130}, True )
  myAbstract.makeModel("Outfile.prism")

