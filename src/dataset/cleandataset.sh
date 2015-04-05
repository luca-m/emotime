#!/bin/bash
## cleandataset.sh
##
## Copyright (C) 2014 luca.mella@studio.unibo.it
##
## Description:
##
##    NA
##
## Usage:
##
##    cleandataset OPTIONS DSETPATH
##
## Parameters:
##    
##    DSETPATH        Dataset path
##
## Options:
##    
##    -h, --help      This help message
##    --faces         Clear faces 
##    --features      Clear features 
##    --classifiers   Clear classifiers 
##
## Example:
##    
##    cleandataset -h
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

# ----------------
# FUNCTIONS
# ----------------


# ----------------
# MAIN
# ----------------
CLEAR_FACES=0
CLEAR_FEATURES=0
CLEAR_CLASSIFIERS=0
# Options parsing
while [ $# -gt 0 ]; do
  case $1 in
    (-h|--help) usage 2>&1;;
    (--faces) CLEAR_FACES=1;shift;;
    (--features) CLEAR_FEATURES=1;shift;;
    (--classifiers) CLEAR_CLASSIFIERS=1;shift;;
    (--) shift; break;;
    (-*) usage "$1: unknown option";;
    (*) break;;
  esac
done

DSET=$1
if [ "$DSET" == "" ]; then
  usage 2>&1
fi

if [ $CLEAR_FACES -eq 1 ]; then
  for f in `ls -1 $DSET/training/faces | xargs`; 
  do
      rm -fr $DSET/training/faces/$f/*
  done
fi

if [ $CLEAR_FEATURES -eq 1 ]; then
  for f in `ls -1 $DSET/training/features | xargs`; 
  do
    rm -fr $DSET/training/features/$f/* 
  done
fi   
  
rm $DSET/trainfiles/*


if [ $CLEAR_CLASSIFIERS -eq 1 ]; then
  clbackup="$DSET/classifiers.$(date +"%Y%m%d-%H%M")"
  mkdir -p "$clbackup"
  mv $DSET/classifiers/* $clbackup
  mkdir $DSET/classifiers/svm
  mkdir $DSET/classifiers/ada
fi

