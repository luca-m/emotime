#!/bin/bash

FDETECTOR="../resources/haarcascade_frontalface_alt2.xml"

totHit=0
tot=0

for emotion in `ls -1 ../dataset/images | xargs`;
do
  total=`ls -1 ../dataset/images/$emotion/*|wc -l`
  hit=$(echo "`ls -1 ../dataset/images/$emotion/*`"|./boost_emo_detector_cli "$FDETECTOR" 32 32 1 5 8 `ls ../dataset/classifiers/*.xml | xargs` 2>/dev/null| grep "Emotion predicted: $emotion" | wc -l)
  echo "$emotion : $hit/$total   -  `echo "scale=10;$hit/$total"|bc`"
  totHit=`expr $totHit + $hit`
  tot=`expr $tot + $total`
done 

echo "Train hit rate: $totHit/$tot   -  `echo "scale=10;$totHit/$tot"|bc`"
