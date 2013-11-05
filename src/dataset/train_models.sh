#!/bin/bash

DS_FOLDER=../dataset
DS_CONFIG=default.cfg



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

echo "1.3) Training with AdaBoost and selecting relevant features"
python2 ./datasetTrain.py $DS_FOLDER 
echo "------------------------"


#
# PHASE 2
# Using relevant features for SVM training.
#



