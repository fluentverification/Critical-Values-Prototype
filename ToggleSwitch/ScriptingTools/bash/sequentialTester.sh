#!/bin/bash

# It is important to define some sort of framework so 
# that if the program terminates early for some reason
# we can pick up where we left off.

# For reference: the control model with property toggle_IPTG_0
# is 3.383226110378722E-13

# 3 states on each species
TETRFIXED=",0,50,1,2,3,4,5,6"
LACIFIXED=",0,60,80,20,36,77,45,26,69"
tempfile="/tmp/temp.prism"
csl=$1

###--- normalize array ---###
read -a TetR_trs <<< $(echo $TETRFIXED | sed 's/^,//' | sed 's/,/ /g')
read -a LacI_trs <<< $(echo $LACIFIXED | sed 's/^,//' | sed 's/,/ /g')

###--- Start Of Script---###

for TETR in {0..50}; do
  # Make sure we aren't checking models that we already have
  # checked
  if echo $TETRFIXED | grep -qw $TETR ; then 
    continue
  fi

  for LACI in {0..80}; do
    # Make sure we aren't checking models that we already have
    # checked
    if echo $LACIFIXED | grep -qw $LACI ; then 
      continue
    fi
    # Make model
    ./toggleSwitch.sh $TETR$TETRFIXED 0 $LACI$LACIFIXED 60
    # Run Prism
    result=$(prism $tempfile $csl | grep "Result: " | sed "s/Result: //" | sed "s/ (value.*//")
    # Write result
    printf "%5d %5d %14.8f\n" $TETR $LACI $result
  done 
done
