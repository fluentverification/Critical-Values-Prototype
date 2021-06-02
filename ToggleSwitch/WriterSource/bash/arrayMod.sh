#!/bin/bash

array_mod () {
arr=($( echo $@ | tr " " "\n" | sort -n ))
#echo ${arr[*]}
return $arr
}


new=(8 1 2 9 3 5)

echo ${new[*]}

new=($( echo ${new[*]} | tr " " "\n" | sort -n )) 

echo ${new[*]}
