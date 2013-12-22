#!/bin/bash

FDETECTOR="../resources/haarcascade_frontalface_alt2.xml"

echo "*** BOOSTEMODETECT CLI"
./boost_emo_detector_cli "$FDETECTOR" 32 32 1 5 8 `ls ../dataset/classifiers/*.xml | xargs`

