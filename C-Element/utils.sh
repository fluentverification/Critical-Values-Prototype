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
      printf "$XX "
      prevrate=$rate
      runningsum=0
      totalstates=$(($totalstates + 1))
    else
      runningsum=$tempsum
    fi
  done
  
  echo "$3 is a critical state"
  echo "Total States: $totalstates"

}

abs () {
  if [[ $# -ne 1 ]]; then 
    echo "Syntax error in absoluteValue"
    exit
  fi
  echo $( sed "s/-//" <<< $1)
}

#args: <prism equation>
bashifyeq () {
  if [[ $# -ne 1 ]]; then 
    echo "Syntax error: $0"
    exit
  fi    

  newexpr=$(echo $1 | sed 's/pow//g')
  newexpr=$(echo $newexpr | sed 's/,/^/g')
  echo $newexpr
}

#args: <bc equation> <Species in question>
getfirstpart () {
  if [[ $# -ne 2 ]]; then 
    echo "Syntax error: $0"
    exit
  fi    

  eq=$(echo $1 | sed "s/$2.*$//")
  eq=$(echo $eq | sed "s/[a-zA-Z]/0/g")
  echo $eq
}

#args: <bc equation> <Species in question>
getsecondpart () {
  if [[ $# -ne 2 ]]; then 
    echo "Syntax error: $0"
    exit
  fi    

  eq=$(echo $1 | sed "s/^.*$2//")
  eq=$(echo $eq | sed "s/[a-zA-Z]/0/g")
  echo $eq
}

#args: <prism equation> <species in question> <max> <tolerance>
find_thresholds () {
  if [[ $# -ne 4 ]]; then 
    echo "Syntax error: $0"
    exit
  fi    

  echo "### Finding Thresholds for $2 ###"

  basheq=$(bashifyeq $1)
  eq1=$(getfirstpart $basheq $2)
  eq2=$(getsecondpart $basheq $2)
  
  indicator=$(findmax $eq1 $eq2 $3)
  indicator=$( echo "$indicator * $4" | bc -l)
  
  print_thresholds $eq1 $eq2 $3 $indicator

}
