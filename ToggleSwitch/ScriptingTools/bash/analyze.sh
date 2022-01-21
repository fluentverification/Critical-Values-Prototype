#!/bin/bash

min=100

while IFS= read -r line; do
  value=$(echo $line | awk '{print $3}')
  if (( $(echo "$value < $min" | bc -l ) )); then
    min=$value 
    echo $line
  fi 
done < $1
