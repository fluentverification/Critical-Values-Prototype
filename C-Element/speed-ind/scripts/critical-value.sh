#!/bin/bash

source "./utils.sh"

if [[ "$#" -ne 1 ]]; then
  echo "Please insert decimal value for tolerance"
  exit
fi

tolerance=$1

######              ######
###  Species S1 in S2  ###
######              ######
eq="(10*(0.099/(1.99+pow((0.5*S1),(2)))*10+0.099/(1.99+pow((0.5*0),(2)))*10)/10)"
species="S1"
max=250

find_thresholds $eq $species $max $tolerance


#######              ######
####  Species S2 in S3  ###
#######              ######
#eq1='(10*(0.099/(1.99+((0.5*S4)^2))*10+0.099/(1.99+((0.5*S2)^2))*10))'
#eq2=')^2)*10+0.099/(1.99+(0.5*0)^2)*10))'
#max=100
#
#indicator=$(findmax $eq1 $eq2 $max)
#indicator=$( echo "$indicator * $tolerance" | bc -l)
#
#echo "#### Species S1 ####"
#print_thresholds $eq1 $eq2 $max $indicator
#echo "$max is the last Thresholds"
#
#######              ######
####  Species S2 in S4  ###
#######              ######
#eq1='(10*(0.099/(1.99+(0.5*'
#eq2=')^2)*10+0.099/(1.99+(0.5*0)^2)*10))'
#max=100
#
#indicator=$(findmax $eq1 $eq2 $max)
#indicator=$( echo "$indicator * $tolerance" | bc -l)
#
#echo "#### Species S1 ####"
#print_thresholds $eq1 $eq2 $max $indicator
#echo "$max is the last Thresholds"
#
#######              ######
####  Species S3 in S2  ###
#######              ######
#eq1='(10*(0.099/(1.99+((0.5*0)^2))*10+0.099/(1.99+((0.5*' 
#eq2=')^2))*10))'
#max=150
#
#indicator=$(findmax $eq1 $eq2 $max)
#indicator=$( echo "$indicator * $tolerance" | bc -l)
#
#echo "#### Species S1 ####"
#print_thresholds $eq1 $eq2 $max $indicator
#echo "$max is the last Thresholds"
#
#######              ######
####  Species S3 in Z   ###
#######              ######
#eq1='(10*(0.099/(1.99+(0.5*'
#eq2=')^2)*10+0.099/(1.99+(0.5*0)^2)*10))'
#max=150
#
#indicator=$(findmax $eq1 $eq2 $max)
#indicator=$( echo "$indicator * $tolerance" | bc -l)
#
#echo "#### Species S1 ####"
#print_thresholds $eq1 $eq2 $max $indicator
#echo "$max is the last Thresholds"
#
#######              ######
####  Species S4 in S3  ###
#######              ######
#eq1='(10*(0.099/(1.99+((0.5*S4)^2))*10+0.099/(1.99+((0.5*S2)^2))*10))'
#eq2=')^2)*10+0.099/(1.99+(0.5*0)^2)*10))'
#max=120
#
#indicator=$(findmax $eq1 $eq2 $max)
#indicator=$( echo "$indicator * $tolerance" | bc -l)
#
#echo "#### Species S1 ####"
#print_thresholds $eq1 $eq2 $max $indicator
#echo "$max is the last Thresholds"
#
#######              ######
####  Species S4 in Z   ###
#######              ######
#eq1='(10*(0.099/(1.99+(0.5*'
#eq2=')^2)*10+0.099/(1.99+(0.5*0)^2)*10))'
#max=120
#
#indicator=$(findmax $eq1 $eq2 $max)
#indicator=$( echo "$indicator * $tolerance" | bc -l)
#
#echo "#### Species S1 ####"
#print_thresholds $eq1 $eq2 $max $indicator
#echo "$max is the last Thresholds"
#
#######              ######
####  Species Y in S1   ###
#######              ######
#eq1='(10*(0.099/(1.99+(0.5*'
#eq2=')^2)*10+0.099/(1.99+(0.5*0)^2)*10))'
#max=90
#
#indicator=$(findmax $eq1 $eq2 $max)
#indicator=$( echo "$indicator * $tolerance" | bc -l)
#
#echo "#### Species S1 ####"
#print_thresholds $eq1 $eq2 $max $indicator
#echo "$max is the last Thresholds"
#
#######              ######
####  Species Z in CC   ###
#######              ######
#eq1='(10*(0.099/(1.99+(0.5*'
#eq2=')^2)*10+0.099/(1.99+(0.5*0)^2)*10))'
#max=90
#
#indicator=$(findmax $eq1 $eq2 $max)
#indicator=$( echo "$indicator * $tolerance" | bc -l)
#
#echo "#### Species S1 ####"
#print_thresholds $eq1 $eq2 $max $indicator
#echo "$max is the last Thresholds"
#
#######              ######
####  Species XX in S1  ###
#######              ######
#eq1='(10*(0.099/(1.99+(0.5*'
#eq2=')^2)*10+0.099/(1.99+(0.5*0)^2)*10))'
#max=90
#
#indicator=$(findmax $eq1 $eq2 $max)
#indicator=$( echo "$indicator * $tolerance" | bc -l)
#
#echo "#### Species S1 ####"
#print_thresholds $eq1 $eq2 $max $indicator
#echo "$max is the last Thresholds"
