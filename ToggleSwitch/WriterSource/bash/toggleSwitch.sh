#!/bin/bash


###--- variables ---###
tmpfile="/tmp/temp.prism"

###--- Pre-run checks ---###
if [[ "$#" -ne 4 ]]; then
  echo "Please append States and initial conditions for TetR and LacI"
  echo "Example: $0 0,15,50,100 50 0,20,60,100 60"
  exit
fi

###--- Module characteristics ---###
TetR_Max=$(echo $1 | awk -F ',' '{print $(NF)}')
LacI_Max=$(echo $3 | awk -F ',' '{print $(NF)}')

TetR_Init=$2
LacI_Init=$4

IFS=' '

read -a TetR_trs <<< $(echo $1 | sed 's/,/ /g')
read -a LacI_trs <<< $(echo $3 | sed 's/,/ /g')

###--- Write preamble ---###
cat << _EOF_ > $tmpfile
ctmc

const int aTc = 0;
const int IPTG = 0;

const double kd = 0.0075;

formula p_t = 0.99/(0.25*(LacI/(0.0496277915632754*aTc + 1))*(LacI/(0.0496277915632754*aTc + 1)) + 1.99);
formula p_l = 0.99/(0.25*(TetR/(0.0496277915632754*IPTG + 1))*(TetR/(0.0496277915632754*IPTG + 1)) + 1.99);

const int MAX_TetR = $TetR_Max;
const int MAX_LacI = $LacI_Max;

_EOF_

###--- Write TetR module ---###
echo "module TetR_def" >> $tmpfile
echo >> $tmpfile
echo "  TetR : [0..MAX_TetR] init $TetR_Init;" >> $tmpfile
echo >> $tmpfile

prev=0
curr=0
for next in "${TetR_trs[@]}"
do
  change=$(( $next - $curr ))
  if [[ $change -ne 0 ]]; then
    change=$(printf %.5f $(bc -l <<< "10 / $change"))
  fi
  repress="p_t * $change : (TetR'=$next);"

  if [[ $next -eq 0 ]]; then
    continue
  elif [[ $curr -eq 0 ]]; then
    echo "  [] TetR=0 -> $repress" >> $tmpfile
  else
    echo "  [] TetR=$curr -> kd * TetR / $(expr $curr - $prev) : (TetR'=$prev) + $repress" >> $tmpfile
  fi

  prev=$curr
  curr=$next
done

echo "  [] TetR=$TetR_Max -> kd * TetR / $(expr $curr - $prev) : (TetR'=$prev);" >> $tmpfile
echo >> $tmpfile
echo "endmodule" >> $tmpfile

echo >> $tmpfile
echo >> $tmpfile
###--- Write LacI module ---###
echo "module LacI_def" >> $tmpfile
echo >> $tmpfile
echo "  LacI : [0..MAX_LacI] init $LacI_Init;" >> $tmpfile
echo >> $tmpfile

prev=0
curr=0
for next in "${LacI_trs[@]}"
do
  change=$(( $next - $curr ))
  if [[ $change -ne 0 ]]; then
    change=$(printf %.5f $(bc -l <<< "10 / $change"))
  fi
  repress="p_l * $change : (LacI'=$next);"

  if [[ $next -eq 0 ]]; then
    continue
  elif [[ $curr -eq 0 ]]; then
    echo "  [] LacI=0 -> $repress" >> $tmpfile
  else
    echo "  [] LacI=$curr -> kd * LacI / $(expr $curr - $prev) : (LacI'=$prev) + $repress" >> $tmpfile
  fi

  prev=$curr
  curr=$next
done

echo "  [] LacI=$LacI_Max -> kd * LacI / $(expr $curr - $prev) : (LacI'=$prev);" >> $tmpfile
echo >> $tmpfile
echo "endmodule" >> $tmpfile


