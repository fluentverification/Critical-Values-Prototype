#!/bin/bash

### This script Creates writes a majority model based on ###
### arrays indicating thresholds for each module         ###

if [[ "$#" -ne 2 ]]; then
  echo "Please insert decimal value for tolerance and a name for the file"
  echo "Example: <tolerance> <something.prism>"
  exit
fi

file=$2
tolerance=$1

echo "Generating Majority model with a fractional tolerance of $tolerance"
echo "outputing file to $file"


source "../../utils.sh"

######              ######
###  Species D        ###
######              ######
species="D"
max=250
D_init=200
eq="(10*(0.099/(1.99+pow((0.5*0),(2)))*10+0.099/(1.99+pow((0.5*D),(2)))*10)/30)"
Dg1=$(find_thresholds $eq $species $max $tolerance)
eq="(10*(0.099/(1.99+pow((0.5*0),(2)))*10+0.099/(1.99+pow((0.5*D),(2)))*10)/30)"
Dg2=$(find_thresholds $eq $species $max $tolerance)
eq="(10*0.099/(1.99+pow((0.5*D),(2)))*10/3)"
Dg3=$(find_thresholds $eq $species $max $tolerance)

D_trs=("$Dg1 $Dg2 $Dg3 $D_init")
D_trs=($(mysort "$D_trs"))

######              ######
###  Species Y        ###
######              ######
species="Y"
max=120
Y_init=0
eq="(10*(0.099/(1.99+pow((0.5*0),(2)))*10+0.099/(1.99+pow((0.5*Y),(2)))*10+0.099/(1.99+pow((0.5*0),(2)))*10)/50)"

Y_trs=$(find_thresholds $eq $species $max $tolerance)
Y_trs=("$Y_trs $Y_init")
Y_trs=($(mysort "$Y_trs"))

######              ######
###  Species Z        ###
######              ######
species="Z"
max=120
Z_init=0
eq="(10*(0.099/(1.99+pow((0.5*0),(2)))*10+0.099/(1.99+pow((0.5*0),(2)))*10+0.099/(1.99+pow((0.5*Z),(2)))*10)/50)"

Z_trs=$(find_thresholds $eq $species $max $tolerance)
Z_trs=("$Z_trs $Z_init")
Z_trs=($(mysort "$Z_trs"))

######              ######
###  Species CC        ###
######              ######
CC_trs=(0 30 60 90 120 150)
CC_init=120

######              ######
###  Species XX        ###
######              ######
species="XX"
max=120
XX_init=0
eq="(10*(0.099/(1.99+pow((0.5*XX),(2)))*10+0.099/(1.99+pow((0.5*0),(2)))*10+0.099/(1.99+pow((0.5*0),(2)))*10)/50)"

XX_trs=$(find_thresholds $eq $species $max $tolerance)
XX_trs=("$XX_trs $XX_init")
XX_trs=($(mysort "$XX_trs"))

######              ######
###  Species EE        ###
######              ######
species="EE"
max=45
EE_init=0
eq="(10*0.099/(1.99+pow((0.5*EE),(2)))*10/10)"

EE_trs=$(find_thresholds $eq $species $max $tolerance)
EE_trs=("$EE_trs $EE_init")
EE_trs=($(mysort "$EE_trs"))


######----- Model Start -----######

cat > $file << _MODEL_
ctmc
_MODEL_

######----- Module D -----######

D_max=250

cat >> $file <<- _MODEL_
module D_def
  D : [0..$D_max] init $D_init;
_MODEL_

prev=0
curr=0
for next in "${D_trs[@]}"
do
  if [[ $next -eq 0 ]]; then 
    continue
  elif [[ $curr -eq 0 ]]; then
    echo "  [] D=0 -> (10*(0.099/(1.99+pow((0.5*XX),(2)))*10+0.099/(1.99+pow((0.5*Y),(2)))*10+0.099/(1.99+pow((0.5*Z),(2)))*10)/$next):(D'=$next);" >> $file
  else
    echo "  [] D=$curr -> (0.0075*D/$(expr $curr - $prev)):(D'=$prev) + (10*(0.099/(1.99+pow((0.5*XX),(2)))*10+0.099/(1.99+pow((0.5*Y),(2)))*10+0.099/(1.99+pow((0.5*Z),(2)))*10)/$(expr $next - $curr)):(D'=$next);" >> $file
  fi

  prev=$curr
  curr=$next
done

echo "  [] D=$D_max -> (0.0075*D/$(expr $curr - $prev)):(D'=$prev);" >> $file
echo "endmodule" >> $file 

######----- Module Y -----######

Y_max=120

cat >> $file <<- _MODEL_
module Y_def
  Y : [0..$Y_max] init $Y_init;
_MODEL_

prev=0
curr=0
for next in "${Y_trs[@]}"
do
  if [[ $next -eq 0 ]]; then 
    continue
  elif [[ $curr -eq 0 ]]; then
    #initial transition
    echo "  [] Y=0 -> (10*(0.099/(1.99+pow((0.5*AA),(2)))*10+0.099/(1.99+pow((0.5*D),(2)))*10)/$next):(Y'=$next);" >> $file
  else
    echo "  [] Y=$curr -> (0.0075*Y/$(expr $curr - $prev)):(Y'=$prev) + (10*(0.099/(1.99+pow((0.5*AA),(2)))*10+0.099/(1.99+pow((0.5*D),(2)))*10)/$(expr $next - $curr)):(Y'=$next);" >> $file
  fi

  prev=$curr
  curr=$next
done

echo "  [] Y=$Y_max -> (0.0075*Y/$(expr $curr - $prev)):(Y'=$prev);" >> $file
echo "endmodule" >> $file 

######----- Module Z -----######

Z_max=120

cat >> $file <<- _MODEL_
module Z_def
  Z : [0..$Z_max] init $Z_init;
_MODEL_

prev=0
curr=0
for next in "${Z_trs[@]}"
do
  if [[ $next -eq 0 ]]; then 
    continue
  elif [[ $curr -eq 0 ]]; then
    #initial transition
    echo "  [] Z=0 -> (10*(0.099/(1.99+pow((0.5*B),(2)))*10+0.099/(1.99+pow((0.5*D),(2)))*10)/$next):(Z'=$next);" >> $file
  else
    echo "  [] Z=$curr -> (0.0075*Z/$(expr $curr - $prev)):(Z'=$prev) + (10*(0.099/(1.99+pow((0.5*B),(2)))*10+0.099/(1.99+pow((0.5*D),(2)))*10)/$( expr $next - $curr)):(Z'=$next);" >> $file
  fi

  prev=$curr
  curr=$next
done

echo "  [] Z=$Z_max -> (0.0075*Z/$(expr $curr - $prev)):(Z'=$prev);" >> $file
echo "endmodule" >> $file 

######----- Module CC -----######

CC_max=150

cat >> $file <<- _MODEL_
module CC_def
  CC : [0..$CC_max] init $CC_init;
_MODEL_

prev=0
curr=0
for next in "${CC_trs[@]}"
do
  if [[ $next -eq 0 ]]; then 
    continue
  elif [[ $curr -eq 0 ]]; then
    #initial transition
    echo "  [] CC=0 -> (10*0.099/(1.99+pow((0.5*EE),(2)))*10/$next):(CC'=$next);" >> $file
  else
    echo "  [] CC=$curr -> (0.0075*CC/$(expr $curr - $prev)):(CC'=$prev) + (10*0.099/(1.99+pow((0.5*EE),(2)))*10/$(expr $next - $curr)):(CC'=$next);" >> $file
  fi

  prev=$curr
  curr=$next
done

echo "  [] CC=$CC_max -> (0.0075*CC/$(expr $curr - $prev)):(CC'=$prev);" >> $file
echo "endmodule" >> $file 

######----- Module XX -----######

XX_max=120

cat >> $file <<- _MODEL_
module XX_def
  XX : [0..$XX_max] init $XX_init;
_MODEL_

prev=0
curr=0
for next in "${XX_trs[@]}"
do
  if [[ $next -eq 0 ]]; then 
    continue
  elif [[ $curr -eq 0 ]]; then
    #initial transition
    echo "  [] XX=0 -> (10*(0.099/(1.99+pow((0.5*AA),(2)))*10+0.099/(1.99+pow((0.5*B),(2)))*10)/$next):(XX'=$next);" >> $file
  else
    echo "  [] XX=$curr -> (0.0075*XX/$(expr $curr - $prev)):(XX'=$prev) + (10*(0.099/(1.99+pow((0.5*AA),(2)))*10+0.099/(1.99+pow((0.5*B),(2)))*10)/$(expr $next - $curr)):(XX'=$next);" >> $file
  fi

  prev=$curr
  curr=$next
done

echo "  [] XX=$XX_max -> (0.0075*XX/$(expr $curr - $prev)):(XX'=$prev);" >> $file
echo "endmodule" >> $file 

######----- Module EE -----######

EE_max=45

cat >> $file <<- _MODEL_
module EE_def
  EE : [0..$EE_max] init $EE_init;
_MODEL_

prev=0
curr=0
for next in "${EE_trs[@]}"
do
  if [[ $next -eq 0 ]]; then 
    continue
  elif [[ $curr -eq 0 ]]; then
    #initial transition
    echo "  [] EE=0 -> (10*0.099/(1.99+pow((0.5*D),(2)))*10/$next):(EE'=$next);" >> $file
  else
    echo "  [] EE=$curr -> (0.0075*EE/$(expr $curr - $prev)):(EE'=$prev) + (10*0.099/(1.99+pow((0.5*D),(2)))*10/$(expr $next - $curr)):(EE'=$next);" >> $file
  fi

  prev=$curr
  curr=$next
done

echo "  [] EE=$EE_max -> (0.0075*EE/$(expr $curr - $prev)):(EE'=$prev);" >> $file
echo "endmodule" >> $file 

######----- Ending stuff -----######

cat >> $file <<- _MODEL_
const int AA=0;
const int B=120;
_MODEL_
