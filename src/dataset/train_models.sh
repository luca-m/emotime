#!/bin/bash

DS_FOLDER=~/Documents/emotime_data/
DS_CONFIG=default.cfg

MODE=adaboost
PREPTRAIN_MODE=1vsAll

if [[ $# -gt 0 ]] ; then
  if [[ $1 == "svm" ]]; then
    MODE=svm
    PREPTRAIN_MODE=1vsAllExt
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
python2 ./datasetPrepTrain.py $DS_FOLDER --mode $PREPTRAIN_MODE
echo "------------------------"

echo "1.3) Training with $MODE and selecting relevant features"
python2 ./datasetTrain.py $DS_FOLDER --mode $MODE
echo "------------------------"

echo "1.4) Verifying the prediction of $MODE"
python2 ./datasetVerifyPrediction.py --mode $MODE $DS_FOLDER
echo "------------------------"

#
# PHASE 2
# Using relevant features for SVM training.
#



