#!/bin/bash

DS_FOLDER=../dataset
DS_CONFIG=default.cfg

MODE=adaboost

if [[ $# -gt 0 ]] ; then
  if [[ $1 == "svm" ]]; then
    MODE=svm
  elif [[ $1 == "adaboost" ]]; then
    MODE=adaboost
  fi
fi

#
# PHASE 1
# Finding relevant features using AdaBoost.
#

echo "1.1) Cropping faces"
python2 ./datasetCropFaces.py $DS_FOLDER 
echo "------------------------"

echo "1.2.a) Calculating features using bank of Gabor magnitude filters"
python2 ./datasetFeatures.py $DS_FOLDER
echo "------------------------"

echo "1.2.b) Preparing CSV file containing training data"
python2 ./datasetPrepTrain.py $DS_FOLDER 
echo "------------------------"

echo "1.3) Training with $MODE and selecting relevant features"
python2 ./datasetTrain.py $DS_FOLDER --mode $MODE
echo "------------------------"


#
# PHASE 2
# Using relevant features for SVM training.
#



