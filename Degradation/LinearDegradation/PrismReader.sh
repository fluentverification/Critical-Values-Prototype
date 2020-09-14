#!/bin/bash

###################################
#Set Variables
###################################

porter=true




###################################
#PreRun checks
###################################

if ! command prism -v &> /dev/null; then
	if porter; then
		echo "sourcing .profile"
		source ~/.profile
	else
		echo "prism command not found"
		echo "aborting"
		exit
	fi
fi

###################################

values=(100 50 25 20 10 5 4 2 1)

for i in "${values[@]}"; do 

	./PrismWriter temp.prism 100 "$i"

	Result="$(prism temp.prism SSDegradation.csl | grep "Result: " | sed s/^Result:.// | sed s/"............(value in the initial state)"//)"

	echo "$Result $i" >> result.txt

done
