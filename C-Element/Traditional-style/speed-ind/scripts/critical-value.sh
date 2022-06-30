#!/bin/bash

source "../../utils.sh"

if [[ "$#" -ne 1 ]]; then
  echo "Please insert decimal value for tolerance"
  exit
fi

tolerance=$1

######              ######
###  Species S1 in S2  ###
######              ######
eq="(10*(0.099/(1.99+pow((0.5*S1),(2)))*10+0.099/(1.99+pow((0.5*0),(2)))*10))"
species="S1"
max=250

echo "$species in S2"
find_thresholds $eq $species $max $tolerance


######              ######
###  Species S2 in S3  ###
######              ######
eq="(10*(0.099/(1.99+pow((0.5*0),(2)))*10+0.099/(1.99+pow((0.5*S2),(2)))*10)/15)"
species="S2"
max=100

echo "$species in S2"
find_thresholds $eq $species $max $tolerance


######              ######
###  Species S2 in S4  ###
######              ######
eq="(10*(0.099/(1.99+pow((0.5*0),(2)))*10+0.099/(1.99+pow((0.5*0),(2)))*10+0.099/(1.99+pow((0.5*S2),(2)))*10)/40)"
species="S2"
max=100

echo "$species in S2"
find_thresholds $eq $species $max $tolerance


######              ######
###  Species S3 in S2  ###
######              ######
eq="(10*(0.099/(1.99+pow((0.5*0),(2)))*10+0.099/(1.99+pow((0.5*S3),(2)))*10)/10)"
species="S3"
max=150

echo "$species in S2"
find_thresholds $eq $species $max $tolerance


######              ######
###  Species S3 in Z   ###
######              ######
eq="(10*(0.099/(1.99+pow((0.5*0),(2)))*10+0.099/(1.99+pow((0.5*S3),(2)))*10)/30)"
species="S3"
max=150

echo "$species in S2"
find_thresholds $eq $species $max $tolerance


######              ######
###  Species S4 in S3  ###
######              ######
eq="(10*(0.099/(1.99+pow((0.5*S4),(2)))*10+0.099/(1.99+pow((0.5*0),(2)))*10)/15)"
species="S4"
max=120

echo "$species in S2"
find_thresholds $eq $species $max $tolerance


######              ######
###  Species S4 in Z   ###
######              ######
eq="(10*(0.099/(1.99+pow((0.5*S4),(2)))*10+0.099/(1.99+pow((0.5*0),(2)))*10)/30)"
species="S4"
max=120

echo "$species in S2"
find_thresholds $eq $species $max $tolerance


######              ######
###  Species Y in S1   ###
######              ######

eq="(10*(0.099/(1.99+pow((0.5*0),(2)))*10+0.099/(1.99+pow((0.5*Y),(2)))*10)/50)"
species="Y"
max=90

echo "$species in S2"
find_thresholds $eq $species $max $tolerance

######              ######
###  Species Z in CC   ###
######              ######
eq="(10*0.099/(1.99+pow((0.5*Z),(2)))*10/40)"
species="Z"
max=90

echo "$species in S2"
find_thresholds $eq $species $max $tolerance


######              ######
###  Species XX in S1  ###
######              ######
eq="(10*(0.099/(1.99+pow((0.5*XX),(2)))*10+0.099/(1.99+pow((0.5*0),(2)))*10)/50)"
species="XX"
max=90

echo "$species in S2"
find_thresholds $eq $species $max $tolerance

