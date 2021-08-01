#!/bin/bash

abs(){
  if [[ $# -ne 1 ]]; then
    echo "Syntax Error in absoluteValue"
    exit 1
  fi

  echo $( sed "s/-//" <<< $1) 
  
}
