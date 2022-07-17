#!/bin/bash

source "./utils.sh"

if [[ "$#" -ne 1 ]]; then
  echo "Please insert decimal value for tolerance"
  exit
fi

tolerance=$1

######                  ######
###  Species TetR in LacI  ###
######                  ######
eq="(0.99/(0.25*(TetR/(0.0496277915632754*0+1))*(TetR/(0.0496277915632754*0+1))+1.99))"
species="TetR"
max=50

echo "$species in LacI"
find_thresholds $eq $species $max $tolerance
echo

######                  ######
###  Species LacI in TetR  ###
######                  ######
eq="(0.99/(0.25*(LacI/(0.0496277915632754*0+1))*(LacI/(0.0496277915632754*0+1))+1.99))"
species="LacI"
max=80

echo "$species in TetR"
find_thresholds $eq $species $max $tolerance
echo
