#!/bin/bash

tmpfile="/tmp/test.prism"

LacIValues=(0 10 20 30 40 50 60 70 80 90 100)
LacIInit=0

TetRValues=(0 20 40 60 80 100)
TetRInit=0

cat << _EOF_ > $tmpfile
ctmc

const int aTc = 0;
const int IPTG = 0;

formula p_t = 0.99/(0.25*(LacI/(0.0496277915632754*aTc + 1))*(LacI/(0.0496277915632754*aTc + 1)) + 1.99);
formula p_l = 0.99/(0.25*(TetR/(0.0496277915632754*IPTG + 1))*(TetR/(0.0496277915632754*IPTG + 1)) + 1.99);

const int MAXTetR = 100;
const int MAXLacI = 100;
_EOF_


