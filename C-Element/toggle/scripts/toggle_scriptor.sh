#!/bin/bash

### This script Creates writes a majority model based on ###
### arrays indicating thresholds for each module         ###

if [[ "$#" -ne 1 ]]; then
  echo "Please insert decimal value for tolerance"
  exit
fi

file="testtoggle.prism"
tolerance=$1

source "../../utils.sh"

######        ######
###  Species D   ###
######        ######
species="D"
max=80
D_init=0
eq="(10*(0.99/(1.99+pow((0.5*D),(2)))+0.99/(1.99+pow((0.5*0),(2))))/50)"

D_trs=$(find_thresholds $eq $species $max $tolerance)
D_trs=("$D_trs $D_init")
D_trs=($(mysort "$D_trs"))

######        ######
###  Species Y   ###
######        ######
species="Y"
max=225
Y_init=195
eq="(10*(0.99/(1.99+pow((0.5*0),(2)))+0.99/(1.99+pow((0.5*Y),(2))))/6)"

Y_trs=$(find_thresholds $eq $species $max $tolerance)
Y_trs=("$Y_trs $Y_init")
Y_trs=($(mysort "$Y_trs"))

######        ######
###  Species Z   ###
######        ######
species="Z"
max=90
Z_init=0
eq="(10*(0.99/(1.99+pow((0.5*0),(2)))+0.99/(1.99+pow((0.5*Z),(2))))/15)"
Zg1=$(find_thresholds $eq $species $max $tolerance)
eq="(10*0.99/(1.99+pow((0.5*Z),(2)))/30)"
Zg2=$(find_thresholds $eq $species $max $tolerance)

Z_trs=("$Zg1 $Zg2 $Z_init")
Z_trs=($(mysort "$Z_trs"))

######           ######
###  Species Cout   ###
######           ######
Cout_trs=(0 30 60 90 120)
Cout_init=120

######         ######
###  Species XX   ###
######         ######
species="XX"
max=80
XX_init=0
eq="(10*(0.99/(1.99+pow((0.5*XX),(2)))+0.99/(1.99+pow((0.5*0),(2))))/15)"

XX_trs=$(find_thresholds $eq $species $max $tolerance)
XX_trs=("$XX_trs $XX_init")
XX_trs=($(mysort "$XX_trs"))

######         ######
###  Species FF   ###
######         ######
species="FF"
max=250
FF_init=200
eq="(10*(0.99/(1.99+pow((0.5*FF),(2)))+0.99/(1.99+pow((0.5*0),(2))))/6)"

FF_trs=$(find_thresholds $eq $species $max $tolerance)
FF_trs=("$FF_trs $FF_init")
FF_trs=($(mysort "$FF_trs"))

######         ######
###  Species EE   ###
######         ######
species="EE"
max=80
EE_init=0
eq="(10*(0.99/(1.99+pow((0.5*0),(2)))+0.99/(1.99+pow((0.5*EE),(2))))/50)"

EE_trs=$(find_thresholds $eq $species $max $tolerance)
EE_trs=("$EE_trs $EE_init")
EE_trs=($(mysort "$EE_trs"))


#### MISC ####

Y_max=${Y_trs[${#Y_trs[@]}-1]}
Z_max=${Z_trs[${#Z_trs[@]}-1]}
Cout_max=${Cout_trs[${#Cout_trs[@]}-1]}
XX_max=${XX_trs[${#XX_trs[@]}-1]}
FF_max=${FF_trs[${#FF_trs[@]}-1]}
D_max=${D_trs[${#D_trs[@]}-1]}
EE_max=${EE_trs[${#EE_trs[@]}-1]}

######----- Model Start -----######

cat > $file << _MODEL_
ctmc
_MODEL_

######----- Module D -----######

cat >> $file <<- _MODEL_
module D_def
  D : [0..$D_max] init $D_init;
_MODEL_

prev=0
curr=0
for next in "${D_trs[@]}"
do
  repress="(10*0.99/(1.99+pow((0.5*AA),(2)))/$(expr $next - $curr)):(D'=$next);"

  if [[ $next -eq 0 ]]; then 
    continue
  elif [[ $curr -eq 0 ]]; then
    echo "  [] D=0 -> $repress" >> $file
  else
    echo "  [] D=$curr -> (D*0.0075/$(expr $curr - $prev)):(D'=$prev) + $repress" >> $file
  fi

  prev=$curr
  curr=$next
done

echo "  [] D=$D_max -> (D*0.0075/$(expr $curr - $prev)):(D'=$prev);" >> $file
echo "endmodule" >> $file 

######----- Module Y -----######

cat >> $file <<- _MODEL_
module Y_def
  Y : [0..$Y_max] init $Y_init;
_MODEL_

prev=0
curr=0
for next in "${Y_trs[@]}"
do
  repress="(10*(0.99/(1.99+pow((0.5*XX),(2)))+0.99/(1.99+pow((0.5*Z),(2))))/$(expr $next - $curr)):(Y'=$next);"

  if [[ $next -eq 0 ]]; then 
    continue
  elif [[ $curr -eq 0 ]]; then
    #initial transition
    echo "  [] Y=0 -> $repress" >> $file
  else
    echo "  [] Y=$curr -> (Y*0.0075/$(expr $curr - $prev)):(Y'=$prev) + $repress" >> $file
  fi

  prev=$curr
  curr=$next
done

echo "  [] Y=$Y_max -> (Y*0.0075/$(expr $curr - $prev)):(Y'=$prev);" >> $file
echo "endmodule" >> $file 

######----- Module Z -----######

cat >> $file <<- _MODEL_
module Z_def
  Z : [0..$Z_max] init $Z_init;
_MODEL_

prev=0
curr=0
for next in "${Z_trs[@]}"
do
  repress="(10*(0.99/(1.99+pow((0.5*FF),(2)))+0.99/(1.99+pow((0.5*Y),(2))))/$(expr $next - $curr)):(Z'=$next);"  

  if [[ $next -eq 0 ]]; then 
    continue
  elif [[ $curr -eq 0 ]]; then
    echo "  [] Z=0 -> $repress" >> $file
  else
    echo "  [] Z=$curr -> (Z*0.0075/$(expr $curr - $prev)):(Z'=$prev) + $repress" >> $file
  fi

  prev=$curr
  curr=$next
done

echo "  [] Z=$Z_max -> (Z*0.0075/$(expr $curr - $prev)):(Z'=$prev);" >> $file
echo "endmodule" >> $file 

######----- Module Cout -----######

cat >> $file <<- _MODEL_
module Cout_def
  Cout : [0..$Cout_max] init $Cout_init;
_MODEL_

prev=0
curr=0
for next in "${Cout_trs[@]}"
do
  repress="(10*0.99/(1.99+pow((0.5*Z),(2)))/$(expr $next - $curr)):(Cout'=$next);"

  if [[ $next -eq 0 ]]; then 
    continue
  elif [[ $curr -eq 0 ]]; then
    echo "  [] Cout=0 -> $repress" >> $file
  else
    echo "  [] Cout=$curr -> (Cout*0.0075/$(expr $curr - $prev)):(Cout'=$prev) + $repress" >> $file
  fi

  prev=$curr
  curr=$next
done

echo "  [] Cout=$Cout_max -> (Cout*0.0075/$(expr $curr - $prev)):(Cout'=$prev);" >> $file
echo "endmodule" >> $file 

######----- Module XX -----######

cat >> $file <<- _MODEL_
module XX_def
  XX : [0..$XX_max] init $XX_init;
_MODEL_

prev=0
curr=0
for next in "${XX_trs[@]}"
do
  repress="(10*(0.99/(1.99+pow((0.5*AA),(2)))+0.99/(1.99+pow((0.5*B),(2))))/$(expr $next - $curr)):(XX'=$next);"

  if [[ $next -eq 0 ]]; then 
    continue
  elif [[ $curr -eq 0 ]]; then
    #initial transition
    echo "  [] XX=0 -> $repress" >> $file
  else
    echo "  [] XX=$curr -> (XX*0.0075/$(expr $curr - $prev)):(XX'=$prev) + $repress" >> $file
  fi

  prev=$curr
  curr=$next
done

echo "  [] XX=$XX_max -> (XX*0.0075/$(expr $curr - $prev)):(XX'=$prev);" >> $file
echo "endmodule" >> $file 

######----- Module FF -----######

cat >> $file <<- _MODEL_
module FF_def
  FF : [0..$FF_max] init $FF_init;
_MODEL_

prev=0
curr=0
for next in "${FF_trs[@]}"
do
  repress="(10*(0.99/(1.99+pow((0.5*D),(2)))+0.99/(1.99+pow((0.5*EE),(2))))/$(expr $next - $curr)):(FF'=$next);"

  if [[ $next -eq 0 ]]; then 
    continue
  elif [[ $curr -eq 0 ]]; then
    #initial transition
    echo "  [] FF=0 -> $repress" >> $file
  else
    echo "  [] FF=$curr -> (FF*0.0075/$(expr $curr - $prev)):(FF'=$prev) + $repress" >> $file
  fi

  prev=$curr
  curr=$next
done

echo "  [] FF=$FF_max -> (FF*0.0075/$(expr $curr - $prev)):(FF'=$prev);" >> $file
echo "endmodule" >> $file 

######----- Module EE -----######

cat >> $file <<- _MODEL_
module EE_def
  EE : [0..$EE_max] init $EE_init;
_MODEL_

prev=0
curr=0
for next in "${EE_trs[@]}"
do
  repress="(10*0.99/(1.99+pow((0.5*B),(2)))/$(expr $next - $curr)):(EE'=$next);"

  if [[ $next -eq 0 ]]; then 
    continue
  elif [[ $curr -eq 0 ]]; then
    echo "  [] EE=0 -> $repress" >> $file
  else
    echo "  [] EE=$curr -> (EE*0.0075/$(expr $curr - $prev)):(EE'=$prev) + $repress" >> $file
  fi

  prev=$curr
  curr=$next
done

echo "  [] EE=$EE_max -> (EE*0.0075/$(expr $curr - $prev)):(EE'=$prev);" >> $file
echo "endmodule" >> $file 

######----- Ending stuff -----######

cat >> $file <<- _MODEL_
const int AA=0;
const int B=120;
_MODEL_
