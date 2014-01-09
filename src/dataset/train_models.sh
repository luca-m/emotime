#!/bin/bash
##
## Description:
##
##    Train script
##
## Usage:
##
##    train_models.sh OPTIONS MODE PREPTRAINMODE DATASET
##
## Parameters:
##    
##    MODE            ada, svm
##    PREPTRAINMODE   1vs1, 1vsall, 1vsallext
##    DATASET         Dataset folder
##
## Options:
##    
##    -h, --help      This help message
##    --no-eye        Do not perform eye-based face rotation correction
##
## Example:
##    
##    ./train_models.sh -h
##

# ----------------
# UTILITY FUNCTIONS
# ----------------

echoerr() { 
  echo "$@" 1>&2; 
}
usage() {
  [ "$*" ] && echoerr "$0: $*"
  sed -n '/^##/,/^$/s/^## \{0,1\}//p' "$0" 1>&2
  exit 2
}

# default params
DS_FOLDER=../dataset
DS_CONFIG=default.cfg
MODE=adaboost
PREPTRAINMODE=1vsAll
EYE="--eye-correction"

# ----------------
# MAIN
# ----------------

# Options parsing
while [ $# -gt 0 ]; do
  case $1 in
    (-h|--help) usage 2>&1;;
    (--no-eye) EYE='';;
    (--) shift; break;;
    (-*) usage "$1: unknown option";;
    (*) break;;
  esac
done

if [[ $# -lt 3 ]] ; then
    echoerr "ERR: Missing mandatory parameter"
    exit -1
fi

# Parameter parsing
if [[ $1 == "svm" ]]; then
  MODE='svm'
elif [[ $1 == "adaboost" ]]; then
  MODE='adaboost'
fi

case ${2,,} in
  "1vsall")
    PREPTRAINMODE='1vsAll'
    ;;
  "1vsallext")
    PREPTRAINMODE='1vsAllExt'
    ;;
  "1vs1")
    PREPTRAINMODE='1vs1'
    ;;
  *)
    ;;
esac

DS_FOLDER="$3"

echo "1) Cropping faces:"
python2 ./datasetCropFaces.py $DS_FOLDER $EYE
echo "------------------------"
echo "2) Calculating features using bank of Gabor magnitude filters:"
python2 ./datasetFeatures.py $DS_FOLDER
echo "------------------------"
# Non dataset dependent standard cleanup
rm $DS_FOLDER/training/*.csv
echo "3) Preparing CSV file containing training data:"
python2 ./datasetPrepTrain.py $DS_FOLDER --mode $PREPTRAINMODE
echo "------------------------"
echo "4) Training with $MODE and selecting relevant features:"
python2 ./datasetTrain.py $DS_FOLDER --mode $MODE
echo "------------------------"
echo "5) Verifying the prediction of $MODE:"
python2 ./datasetVerifyPrediction.py --mode $MODE $DS_FOLDER $EYE
echo "------------------------"

