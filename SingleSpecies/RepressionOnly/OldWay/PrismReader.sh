#!/bin/bash

###################################
#Set Variables
###################################
#Name of model
name="Repression only Old Way"

#This is only for Porter
porter=true

#Max value for Prism models
max=100

#These are the epsilon values for prism
values=(100 50 25 20 10 5 4 2 1)

#Name of resulting file (without .txt)
file="Oldresult"

#Destination of result
location="/home/porter/Critical-Values-Prototype/SingleSpecies/RepressionOnly"


###################################
#Help/usage
###################################

print_usage(){
	echo "usage..."
}

###################################
#flags
###################################

verbose=false
while test $# -gt 0; do
	case "$1" in
		'-v')
			verbose=true
			shift
			;;
		'-h'|'--help')
			print_usage
			exit 1
			;;
		*)
			print_usage
			exit 1
			;;
	esac
done


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

#Print the model about to be tested
if "$verbose"; then
	echo "Running model \"$name\""
fi

#Run prism for each value of set values
for i in "${values[@]}"; do 

	#Print status
	if "$verbose"; then
		echo "Checking model with $i values"
	fi

	#Run Prism for next value of set values 
	./PrismWriter  temp.prism "$max" "$i"

	Result="$(prism temp.prism ../SSRepression.csl | grep "Result: " | sed s/^Result:.// | sed s/".(value in the initial state)"//)"

	echo "$Result $(expr $max / $i)" >> "$location/$file".txt

done

rm temp.prism
