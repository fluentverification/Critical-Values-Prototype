#!/bin/bash

### This script Creates writes a majority model based on ###
### arrays indicating thresholds for $speciesh module         ###

if [[ "$#" -ne 1 ]]; then
  echo "Please insert decimal value for tolerance"
  exit
fi

file="testspeed-ind.prism"
tolerance=$1

source "../../utils.sh"

######              ######
###  Species S1        ###
######              ######
species="S1"
max=250
S1_init=200
eq="(10*(0.099/(1.99+pow((0.5*S1),(2)))*10+0.099/(1.99+pow((0.5*0),(2)))*10))"

S1_trs=$(find_thresholds $eq $species $max $tolerance)
S1_trs=("$S1_trs $S1_init")
S1_trs=($(mysort "$S1_trs"))

######              ######
###  Species S2        ###
######              ######
species="S2"
max=100
S2_init=0

eq="(10*(0.099/(1.99+pow((0.5*0),(2)))*10+0.099/(1.99+pow((0.5*S2),(2)))*10)/15)"
S2g1=$(find_thresholds $eq $species $max $tolerance)

eq="(10*(0.099/(1.99+pow((0.5*0),(2)))*10+0.099/(1.99+pow((0.5*0),(2)))*10+0.099/(1.99+pow((0.5*S2),(2)))*10)/40)"
S2g2=$(find_thresholds $eq $species $max $tolerance)

S2_trs=("$S2g1 $S2_init $S2g2")
S2_trs=($(mysort "$S2_trs"))

######              ######
###  Species S3        ###
######              ######
species="S3"
max=150
S3_init=120

eq="(10*(0.099/(1.99+pow((0.5*0),(2)))*10+0.099/(1.99+pow((0.5*S3),(2)))*10)/10)"
S3g1=$(find_thresholds $eq $species $max $tolerance)

eq="(10*(0.099/(1.99+pow((0.5*0),(2)))*10+0.099/(1.99+pow((0.5*S3),(2)))*10)/30)"
S3g2=$(find_thresholds $eq $species $max $tolerance)

S3_trs=("$S3g1 $S3_init $S3g2")
S3_trs=($(mysort "$S3_trs"))

######              ######
###  Species S4        ###
######              ######
species="S4"
max=120
S4_init=0

eq="(10*(0.099/(1.99+pow((0.5*S4),(2)))*10+0.099/(1.99+pow((0.5*0),(2)))*10)/15)"
S4g1=$(find_thresholds $eq $species $max $tolerance)

eq="(10*(0.099/(1.99+pow((0.5*S4),(2)))*10+0.099/(1.99+pow((0.5*0),(2)))*10)/30)"
S4g2=$(find_thresholds $eq $species $max $tolerance)

S4_trs=("$S4g1 $S4_init $S4g2")
S4_trs=($(mysort "$S4_trs"))

######              ######
###  Species Y         ###
######              ######
species="Y"
max=90
Y_init=0
eq="(10*(0.099/(1.99+pow((0.5*0),(2)))*10+0.099/(1.99+pow((0.5*Y),(2)))*10)/50)"

Y_trs=$(find_thresholds $eq $species $max $tolerance)
Y_trs=("$Y_trs $Y_init")
Y_trs=($(mysort "$Y_trs"))

######              ######
###  Species Z         ###
######              ######
species="Z"
max=90
Z_init=0
eq="(10*0.099/(1.99+pow((0.5*Z),(2)))*10/40)"

Z_trs=$(find_thresholds $eq $species $max $tolerance)
Z_trs=("$Z_trs $Z_init")
Z_trs=($(mysort "$Z_trs"))

######              ######
###  Species CC        ###
######              ######
CC_trs=(0 40 80 120)
CC_init=120

######              ######
###  Species XX        ###
######              ######
species="XX"
max=90
XX_init=0
eq="(10*(0.099/(1.99+pow((0.5*XX),(2)))*10+0.099/(1.99+pow((0.5*0),(2)))*10)/50)"

XX_trs=$(find_thresholds $eq $species $max $tolerance)
XX_trs=("$XX_trs $XX_init")
XX_trs=($(mysort "$XX_trs"))


### MISC ###

S1_max=${S1_trs[${#S1_trs[@]}-1]}
S2_max=${S2_trs[${#S2_trs[@]}-1]}
S3_max=${S3_trs[${#S3_trs[@]}-1]}
S4_max=${S4_trs[${#S4_trs[@]}-1]}
Y_max=${Y_trs[${#Y_trs[@]}-1]}
Z_max=${Z_trs[${#Z_trs[@]}-1]}
CC_max=${CC_trs[${#CC_trs[@]}-1]}
XX_max=${XX_trs[${#XX_trs[@]}-1]}

######----- Model Start -----######

cat > $file << _MODEL_
ctmc
_MODEL_

######----- Module S1 -----######

cat >> $file <<- _MODEL_
module S1_def
  S1 : [0..$S1_max] init $S1_init;
_MODEL_

prev=0
curr=0
for next in "${S1_trs[@]}"
do
  repress="(10*(0.099/(1.99+pow((0.5*XX),(2)))*10+0.099/(1.99+pow((0.5*Y),(2)))*10)/$(expr $next - $curr)):(S1'=$next);"

  if [[ $next -eq 0 ]]; then 
    continue
  elif [[ $curr -eq 0 ]]; then
    echo "  [] S1=0 -> $repress" >> $file
  else
    echo "  [] S1=$curr -> (0.0075*S1/$(expr $curr - $prev)):(S1'=$prev) + $repress" >> $file
  fi

  prev=$curr
  curr=$next
done

echo "  [] S1=$S1_max -> (0.0075*S1/$(expr $curr - $prev)):(S1'=$prev);" >> $file
echo "endmodule" >> $file 

######----- Module S2 -----######

cat >> $file <<- _MODEL_
module S2_def
  S2 : [0..$S2_max] init $S2_init;
_MODEL_

prev=0
curr=0
for next in "${S2_trs[@]}"
do
  repress="(10*(0.099/(1.99+pow((0.5*S1),(2)))*10+0.099/(1.99+pow((0.5*S3),(2)))*10)/$(expr $next - $curr)):(S2'=$next);"

  if [[ $next -eq 0 ]]; then 
    continue
  elif [[ $curr -eq 0 ]]; then
    #initial transition
    echo "  [] S2=0 -> $repress" >> $file
  else
    echo "  [] S2=$curr -> (0.0075*S2/$(expr $curr - $prev)):(S2'=$prev) + $repress" >> $file
  fi

  prev=$curr
  curr=$next
done

echo "  [] S2=$S2_max -> (0.0075*S2/$(expr $curr - $prev)):(S2'=$prev);" >> $file
echo "endmodule" >> $file 

######----- Module S3 -----######

cat >> $file <<- _MODEL_
module S3_def
  S3 : [0..$S3_max] init $S3_init;
_MODEL_

prev=0
curr=0
for next in "${S3_trs[@]}"
do
  repress="(10*(0.099/(1.99+pow((0.5*S4),(2)))*10+0.099/(1.99+pow((0.5*S2),(2)))*10)/$(expr $next - $curr)):(S3'=$next);"  

  if [[ $next -eq 0 ]]; then 
    continue
  elif [[ $curr -eq 0 ]]; then
    echo "  [] S3=0 -> $repress" >> $file
  else
    echo "  [] S3=$curr -> (0.0075*S3/$(expr $curr - $prev)):(S3'=$prev) + $repress" >> $file
  fi

  prev=$curr
  curr=$next
done

echo "  [] S3=$S3_max -> (0.0075*S3/$(expr $curr - $prev)):(S3'=$prev);" >> $file
echo "endmodule" >> $file 

######----- Module S4 -----######

cat >> $file <<- _MODEL_
module S4_def
  S4 : [0..$S4_max] init $S4_init;
_MODEL_

prev=0
curr=0
for next in "${S4_trs[@]}"
do
  repress="(10*(0.099/(1.99+pow((0.5*AA),(2)))*10+0.099/(1.99+pow((0.5*B),(2)))*10+0.099/(1.99+pow((0.5*S2),(2)))*10)/$(expr $next - $curr)):(S4'=$next);"

  if [[ $next -eq 0 ]]; then 
    continue
  elif [[ $curr -eq 0 ]]; then
    echo "  [] S4=0 -> $repress" >> $file
  else
    echo "  [] S4=$curr -> (0.0075*S4/$(expr $curr - $prev)):(S4'=$prev) + $repress" >> $file
  fi

  prev=$curr
  curr=$next
done

echo "  [] S4=$S4_max -> (0.0075*S4/$(expr $curr - $prev)):(S4'=$prev);" >> $file
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
  repress="(10*0.099/(1.99+pow((0.5*B),(2)))*10/$(expr $next - $curr)):(Y'=$next);"

  if [[ $next -eq 0 ]]; then 
    continue
  elif [[ $curr -eq 0 ]]; then
    #initial transition
    echo "  [] Y=0 -> $repress" >> $file
  else
    echo "  [] Y=$curr -> (0.0075*Y/$(expr $curr - $prev)):(Y'=$prev) + $repress" >> $file
  fi

  prev=$curr
  curr=$next
done

echo "  [] Y=$Y_max -> (0.0075*Y/$(expr $curr - $prev)):(Y'=$prev);" >> $file
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
  repress="(10*(0.099/(1.99+pow((0.5*S4),(2)))*10+0.099/(1.99+pow((0.5*S3),(2)))*10)/$(expr $next - $curr)):(Z'=$next);"

  if [[ $next -eq 0 ]]; then 
    continue
  elif [[ $curr -eq 0 ]]; then
    #initial transition
    echo "  [] Z=0 -> $repress" >> $file
  else
    echo "  [] Z=$curr -> (0.0075*Z/$(expr $curr - $prev)):(Z'=$prev) + $repress" >> $file
  fi

  prev=$curr
  curr=$next
done

echo "  [] Z=$Z_max -> (0.0075*Z/$(expr $curr - $prev)):(Z'=$prev);" >> $file
echo "endmodule" >> $file 

######----- Module CC -----######

cat >> $file <<- _MODEL_
module CC_def
  CC : [0..$CC_max] init $CC_init;
_MODEL_

prev=0
curr=0
for next in "${CC_trs[@]}"
do
  repress="(10*0.099/(1.99+pow((0.5*Z),(2)))*10/$(expr $next - $curr)):(CC'=$next);"

  if [[ $next -eq 0 ]]; then 
    continue
  elif [[ $curr -eq 0 ]]; then
    echo "  [] CC=0 -> $repress" >> $file
  else
    echo "  [] CC=$curr -> (0.0075*CC/$(expr $curr - $prev)):(CC'=$prev) + $repress" >> $file
  fi

  prev=$curr
  curr=$next
done

echo "  [] CC=$CC_max -> (0.0075*CC/$(expr $curr - $prev)):(CC'=$prev);" >> $file
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
  repress="(10*0.099/(1.99+pow((0.5*AA),(2)))*10/$(expr $next - $curr)):(XX'=$next);"

  if [[ $next -eq 0 ]]; then 
    continue
  elif [[ $curr -eq 0 ]]; then
    echo "  [] XX=0 -> $repress" >> $file
  else
    echo "  [] XX=$curr -> (0.0075*XX/$(expr $curr - $prev)):(XX'=$prev) + $repress" >> $file
  fi

  prev=$curr
  curr=$next
done

echo "  [] XX=$XX_max -> (0.0075*XX/$(expr $curr - $prev)):(XX'=$prev);" >> $file
echo "endmodule" >> $file 

######----- Ending stuff -----######

cat >> $file <<- _MODEL_
const int AA=0;
const int B=120;
_MODEL_
