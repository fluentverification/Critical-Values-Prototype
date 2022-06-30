#!/bin/bash

source "../../utils.sh"

if [[ "$#" -ne 1 ]]; then
  echo "Please insert decimal value for tolerance"
  exit
fi

tolerance=$1

######              ######
###  Species D in FF  ###
######              ######
eq="(10*(0.99/(1.99+pow((0.5*D),(2)))+0.99/(1.99+pow((0.5*EE),(2))))/50)"
species="D"
max=80

echo "$species in S2"
find_thresholds $eq $species $max $tolerance


######              ######
###  Species Y in Z   ###
######              ######
eq="(10*(0.99/(1.99+pow((0.5*FF),(2)))+0.99/(1.99+pow((0.5*Y),(2))))/6)"
species="Y"
max=225

echo "$species in S2"
find_thresholds $eq $species $max $tolerance


######              ######
###  Species Z in Y   ###
######              ######
eq="(10*(0.99/(1.99+pow((0.5*XX),(2)))+0.99/(1.99+pow((0.5*Z),(2))))/15)"
species="Z"
max=90

echo "$species in S2"
find_thresholds $eq $species $max $tolerance


######              ######
###  Species Z in Cout ###
######              ######
eq="(10*0.99/(1.99+pow((0.5*Z),(2)))/30)"
species="Z"
max=90

echo "$species in S2"
find_thresholds $eq $species $max $tolerance


######              ######
###  Species XX in Y  ###
######              ######
eq="(10*(0.99/(1.99+pow((0.5*XX),(2)))+0.99/(1.99+pow((0.5*Z),(2))))/15)"
species="XX"
max=80

echo "$species in S2"
find_thresholds $eq $species $max $tolerance


######              ######
###  Species FF in Z   ###
######              ######
eq="(10*(0.99/(1.99+pow((0.5*FF),(2)))+0.99/(1.99+pow((0.5*Y),(2))))/6)"
species="FF"
max=250

echo "$species in S2"
find_thresholds $eq $species $max $tolerance


######              ######
###  Species EE in FF  ###
######              ######
eq="(10*(0.99/(1.99+pow((0.5*D),(2)))+0.99/(1.99+pow((0.5*EE),(2))))/50)"
species="EE"
max=80

echo "$species in S2"
find_thresholds $eq $species $max $tolerance
