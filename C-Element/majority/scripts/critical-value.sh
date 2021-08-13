#!/bin/bash

source "../../utils.sh"

if [[ "$#" -ne 1 ]]; then
  echo "Please insert decimal value for tolerance"
  exit
fi

tolerance=$1

######              ######
###  Species D in Y  ###
######              ######
eq="(10*(0.099/(1.99+pow((0.5*AA),(2)))*10+0.099/(1.99+pow((0.5*D),(2)))*10)/30)"
species="D"
max=250


find_thresholds $eq $species $max $tolerance


######              ######
###  Species D in Z  ###
######              ######
eq="(10*(0.099/(1.99+pow((0.5*B),(2)))*10+0.099/(1.99+pow((0.5*D),(2)))*10)/30)"
species="D"
max=250


find_thresholds $eq $species $max $tolerance


######              ######
###  Species D in EE  ###
######              ######
eq="(10*0.099/(1.99+pow((0.5*D),(2)))*10/3)"
species="D"
max=250


find_thresholds $eq $species $max $tolerance


######              ######
###  Species Y in D    ###
######              ######
eq="(10*(0.099/(1.99+pow((0.5*XX),(2)))*10+0.099/(1.99+pow((0.5*Y),(2)))*10+0.099/(1.99+pow((0.5*Z),(2)))*10)/50)"
species="Y"
max=120


find_thresholds $eq $species $max $tolerance


######              ######
###  Species Z in D   ###
######              ######
eq="(10*(0.099/(1.99+pow((0.5*XX),(2)))*10+0.099/(1.99+pow((0.5*Y),(2)))*10+0.099/(1.99+pow((0.5*Z),(2)))*10)/50)"
species="Z"
max=120


find_thresholds $eq $species $max $tolerance


######              ######
###  Species XX in D   ###
######              ######
eq="(10*(0.099/(1.99+pow((0.5*XX),(2)))*10+0.099/(1.99+pow((0.5*Y),(2)))*10+0.099/(1.99+pow((0.5*Z),(2)))*10)/50)"
species="XX"
max=120


find_thresholds $eq $species $max $tolerance


######              ######
###  Species EE in CC  ###
######              ######
eq="(10*0.099/(1.99+pow((0.5*EE),(2)))*10/10)"
species="EE"
max=45


find_thresholds $eq $species $max $tolerance
