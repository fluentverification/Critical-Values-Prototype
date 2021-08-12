#!/bin/bash

#args: <equationpt1> <equationpt2> <max>
findmax () {
  if [[ $# -ne 3 ]]; then 
    echo "Not enough args"
    exit
  fi    

  max=0
  for XX in $(seq 0 $3);
  do
    rate=$(echo $1$XX$2 | bc -l) 
  
    if (( $(echo " $rate > $max " | bc -l) )); then
      max=$rate 
    fi
  done

  echo $max
}

#args: <equationpt1> <equationpt2> <max> <indicator>
print_thresholds () {
  if [[ $# -ne 4 ]]; then 
    echo "Not enough args"
    exit
  fi    

  totalstates=1
  prevrate=0
  runningsum=0
  maxrate=0
  for XX in $(seq 0 $3);
  do
    rate=$(echo $1$XX$2 | bc -l) 
  
    difference=$(echo "$prevrate - $rate" | bc -l )
    tempsum=$(echo "$(abs $difference) + $runningsum" | bc -l)

    if (( $(echo " $tempsum > $indicator" | bc -l) )); then
      echo "$XX is a critical state"
      prevrate=$rate
      runningsum=0
      totalstates=$(($totalstates + 1))
    else
      runningsum=$tempsum
    fi
  done
  
  echo "Total States: $totalstates"

}

abs () {
  if [[ $# -ne 1 ]]; then 
    echo "Syntax error in absoluteValue"
    exit
  fi
  echo $( sed "s/-//" <<< $1)
}

if [[ "$#" -ne 1 ]]; then
  echo "Please insert decimal value for tolerance"
  exit
fi

tolerance=$1

######              ######
###  Species S1 in S2  ###
######              ######
eq1='(10*(0.099/(1.99+((0.5*'
eq2=')^2))*10+0.099/(1.99+((0.5*0)^2))*10))'
max=250

indicator=$(findmax $eq1 $eq2 $max)
indicator=$( echo "$indicator * $tolerance" | bc -l)

echo "#### Species S1 in S2 ####"
print_thresholds $eq1 $eq2 $max $indicator
echo "$max is the last Thresholds"

######              ######
###  Species S2 in S3  ###
######              ######
eq1='(10*(0.099/(1.99+((0.5*S4)^2))*10+0.099/(1.99+((0.5*S2)^2))*10))'
eq2=')^2)*10+0.099/(1.99+(0.5*0)^2)*10))'
max=100

indicator=$(findmax $eq1 $eq2 $max)
indicator=$( echo "$indicator * $tolerance" | bc -l)

echo "#### Species S1 ####"
print_thresholds $eq1 $eq2 $max $indicator
echo "$max is the last Thresholds"

######              ######
###  Species S2 in S4  ###
######              ######
eq1='(10*(0.099/(1.99+(0.5*'
eq2=')^2)*10+0.099/(1.99+(0.5*0)^2)*10))'
max=100

indicator=$(findmax $eq1 $eq2 $max)
indicator=$( echo "$indicator * $tolerance" | bc -l)

echo "#### Species S1 ####"
print_thresholds $eq1 $eq2 $max $indicator
echo "$max is the last Thresholds"

######              ######
###  Species S3 in S2  ###
######              ######
eq1='(10*(0.099/(1.99+((0.5*0)^2))*10+0.099/(1.99+((0.5*' 
eq2=')^2))*10))'
max=150

indicator=$(findmax $eq1 $eq2 $max)
indicator=$( echo "$indicator * $tolerance" | bc -l)

echo "#### Species S1 ####"
print_thresholds $eq1 $eq2 $max $indicator
echo "$max is the last Thresholds"

######              ######
###  Species S3 in Z   ###
######              ######
eq1='(10*(0.099/(1.99+(0.5*'
eq2=')^2)*10+0.099/(1.99+(0.5*0)^2)*10))'
max=150

indicator=$(findmax $eq1 $eq2 $max)
indicator=$( echo "$indicator * $tolerance" | bc -l)

echo "#### Species S1 ####"
print_thresholds $eq1 $eq2 $max $indicator
echo "$max is the last Thresholds"

######              ######
###  Species S4 in S3  ###
######              ######
eq1='(10*(0.099/(1.99+((0.5*S4)^2))*10+0.099/(1.99+((0.5*S2)^2))*10))'
eq2=')^2)*10+0.099/(1.99+(0.5*0)^2)*10))'
max=120

indicator=$(findmax $eq1 $eq2 $max)
indicator=$( echo "$indicator * $tolerance" | bc -l)

echo "#### Species S1 ####"
print_thresholds $eq1 $eq2 $max $indicator
echo "$max is the last Thresholds"

######              ######
###  Species S4 in Z   ###
######              ######
eq1='(10*(0.099/(1.99+(0.5*'
eq2=')^2)*10+0.099/(1.99+(0.5*0)^2)*10))'
max=120

indicator=$(findmax $eq1 $eq2 $max)
indicator=$( echo "$indicator * $tolerance" | bc -l)

echo "#### Species S1 ####"
print_thresholds $eq1 $eq2 $max $indicator
echo "$max is the last Thresholds"

######              ######
###  Species Y in S1   ###
######              ######
eq1='(10*(0.099/(1.99+(0.5*'
eq2=')^2)*10+0.099/(1.99+(0.5*0)^2)*10))'
max=90

indicator=$(findmax $eq1 $eq2 $max)
indicator=$( echo "$indicator * $tolerance" | bc -l)

echo "#### Species S1 ####"
print_thresholds $eq1 $eq2 $max $indicator
echo "$max is the last Thresholds"

######              ######
###  Species Z in CC   ###
######              ######
eq1='(10*(0.099/(1.99+(0.5*'
eq2=')^2)*10+0.099/(1.99+(0.5*0)^2)*10))'
max=90

indicator=$(findmax $eq1 $eq2 $max)
indicator=$( echo "$indicator * $tolerance" | bc -l)

echo "#### Species S1 ####"
print_thresholds $eq1 $eq2 $max $indicator
echo "$max is the last Thresholds"

######              ######
###  Species XX in S1  ###
######              ######
eq1='(10*(0.099/(1.99+(0.5*'
eq2=')^2)*10+0.099/(1.99+(0.5*0)^2)*10))'
max=90

indicator=$(findmax $eq1 $eq2 $max)
indicator=$( echo "$indicator * $tolerance" | bc -l)

echo "#### Species S1 ####"
print_thresholds $eq1 $eq2 $max $indicator
echo "$max is the last Thresholds"
