#!/bin/bash

H=32
W=32
NW=1
NL=5
NT=8

DSFOLDER="../dataset"

for emotion in `ls -1 $DSFOLDER/faces|xargs`;
do
  for face in `ls -1 $DSFOLDER/faces/$emotion/|xargs`;
  do
    ./gaborbank_cli $W $H $NW $NL $NT "$DSFOLDER/faces/$emotion/$face" "$DSFOLDER/features/$emotion/$face"
  done
done 
