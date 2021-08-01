#!/bin/bash
# This bash script contains the necessary components to find the critical 
# values of a species given the equations it controls

#Program includes:
. mathfunctions.sh
set -e

# For example, In the Majority model, Species XX plays apart in controlling
# the reaction rate of D
# To find the Critical values for XX we need to find the states of XX that
# cause the greatest effect on D 
# So, to find the values for XX we need to test the equation for D

### Example: finding points for XX from equation D ###
# equation D: "10/50 * ((.099/(1.99+((.5*$XX)^2)))*10 + \
#              (.099/(1.99+((.5*$Y)^2)))*10 + (.099/(1.99+((.5*$Z)^2)))*10)"
# Here we can see that there is actually 3 contorlling variables in this one 
# equation. But we are only determining XX from this so we can keep Y and Z
# at fixed values. 
## NOTE: We will want to prove that this method selects the same states as 
##       thresholds regardless of what values the other variables are fixed at
## NOTE: We will be using $(bc -l) to do calculations

### Method 1: is the difference between the previous thresholds rate and the 
### current state greater than the defined indicator? If yes then make
### the current state a threshold

echo "|---- Method 1 -----------------------------|"
Y=0
Z=0
indicator=".005"
prevstate=0
prevrate=0
for XX in {0..120..1};
do
  currrate=$(echo "10/50 * ((.099/(1.99+((.5*$XX)^2)))*10 + \
             (.099/(1.99+((.5*$Y)^2)))*10 + (.099/(1.99+((.5*$Z)^2)))*10)" \
             | bc -l)

  difference=$(echo "$prevrate - $currrate" | bc -l)
  #echo $(abs $difference)

  if (( $(echo " $(abs $difference) > $indicator" | bc -l) )); then
    echo "$XX is a critical state "
    prevstate=$XX
    prevrate=$currrate
  fi
done
             
### Method 2: this method adds the difference to a running sum
### if that running sum is greater than the indicator then we make 
### a point and reset the running sum
echo "|---- Method 2 -----------------------------|"

Y=0
Z=0
indicator=".0125"
prevstate=0
prevrate=0
runningsum=0
for XX in {0..120..1};
do
  currrate=$(echo "10/50 * ((.099/(1.99+((.5*$XX)^2)))*10 + \
             (.099/(1.99+((.5*$Y)^2)))*10 + (.099/(1.99+((.5*$Z)^2)))*10)" \
             | bc -l)

  difference=$(echo "$prevrate - $currrate" | bc -l)
  tempsum=$(echo "$(abs $difference) + $runningsum " | bc -l)

  if (( $(echo " $tempsum > $indicator" | bc -l) )); then
    echo "$XX is a critical state "
    prevstate=$XX
    prevrate=$currrate
    runningsum=0
  else
    runningsum=$tempsum 
  fi
done

