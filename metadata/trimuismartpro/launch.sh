#!/bin/sh
echo $0 $*
PROGDIR=/mnt/SDCARD/Apps/formula_1

cd $PROGDIR
HOME=$PROGDIR/ ./game -f
