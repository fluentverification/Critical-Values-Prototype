#!/bin/bash

###################################
#Set Variables
###################################

#This is only for Porter
porter=false

#Max value for Prism models
max=100

#These are the epsilon values for prism
values=(100 50 25 20 10 5 4 2 1)

#Name of resulting file
file="result"

###################################
#PreRun checks
###################################

#Check for the prism command
if ! command prism -v &> /dev/null; then
	if $porter; then
		echo "sourcing .profile"
		source ~/.profile
	else
		echo "prism command not found"
		echo "aborting"
		exit
	fi
fi

###################################
#Run Prism 
###################################

#Run prism for each value of set values
for i in "${values[@]}"; do 

	#Run Prism for next value of set values 
	./PrismWriter temp.prism "$max" "$i"

	Result="$(prism temp.prism SSDegradation.csl | grep "Result: " | sed s/^Result:.// | sed s/"............(value in the initial state)"//)"

	echo "$Result $i" >> "$file".txt

done

rm temp.prism
