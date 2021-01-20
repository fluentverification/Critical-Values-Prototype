#! /bin/bash

###################################
#Set Variables
###################################
#Name of model
#name=""

#Max value for Prism models
max=100

#variables needed:
method=(0 1)

#These are the epsilon values for the evenly spaced models
epsilon_values=(100 50 25 20 10 5 4 2 1)

#These are the number of thresholds for unevenly spaced models
uneven_values=(1 2 4 5 10 20 25 50 100)


#the property to be tested (see the -p flag for manually entering in property)
property="SSRepression.csl"

#Name of resulting file (without .txt)
#file="NewResult"

#Destination of result
#location="/home/porter/Critical-Values-Prototype/SingleSpecies/complete/results"
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"


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
		'-p')
			property="$1"
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
	echo "prism command not found"
	echo "aborting"
	exit
fi


###################################
#Run Prism 
###################################

if "$verbose"; then
	echo "starting even threshold check"
fi


# Run for even thresholds
for m in "${method[@]}"; do
   
	#Print status
	if "$verbose"; then
		echo " ->Starting model check on method $m"
	fi

	#Run prism for each value of set values
	for i in "${epsilon_values[@]}"; do 

		#Print status
		if "$verbose"; then
			echo "  ->Checking model with $i values"
		fi

		#Run Prism for next value of set values 
		../../SingleSpeciesWriter temp.prism "$m" -s "$max" "$i"

		Result="$(prism temp.prism $property | grep "Result: " | sed s/^Result:.// | sed s/".(value in the initial state)"//)"

		echo "$Result $(expr $max / $i)" >> "$DIR/method_$m"_Even.txt

		rm temp.prism

	done

done


#if "$verbose"; then
#	echo "starting un-even threshold check"
#fi
#
## Run for UN-even thresholds
#for m in "${method[@]}"; do
#   
#	#Print status
#	if "$verbose"; then
#		echo " ->Starting model check on method $m"
#	fi
#
#	#Run prism for each value of set values
#	for i in "${uneven_values[@]}"; do 
#
#		#Print status
#		if "$verbose"; then
#			echo "  ->Checking model with $i values"
#		fi
#
#		#Run Prism for next value of set values 
#		../../SingleSpeciesWriter temp.prism "$m" -v "$(../../spacingFinder $i)"
#
#		Result="$(prism temp.prism $property | grep "Result: " | sed s/^Result:.// | sed s/".(value in the initial state)"//)"
#
#		echo "$Result $i" >> "$DIR/method_$m"_Uneven.txt
#
#		rm temp.prism
#
#	done
#
#done

