#!/bin/bash

# It is important to define some sort of framework so 
# that if the program terminates early for some reason
# we can pick up where we left off.

# 3 states on each species
TETRFIXED=",0,50,1"
LACIFIXED=",0,60,80,20"
tempfile="/tmp/temp.prism"
csl=$1

###--- Gather Results ---###
TETR=33
LACI=67
echo "./toggleSwitch.sh $TETR$TETRFIXED 0 $LACI$LACIFIXED 60" 
./toggleSwitch.sh $TETR$TETRFIXED 0 $LACI$LACIFIXED 60
exit


###--- Start Of Script---###

for TETR in {0..50}; do
  for LACI in {0..80}; do
    # Make model
    ./toggleSwitch.sh $TETR$TETRFIXED 0 $LACI$LACIFIXED 60
    # Run Prism
    result=$(prism $tempfile $csl | grep "Result: " | sed "s/Result: //" | sed "s/ (value.*//")
    # Write result
    printf "%5d %5d %14.8f\n" $TETR $LACI $result
  done 
done
