#!/usr/bin/env python2
#import sys
import os
import subprocess
import argparse

from os.path import join
from os.path import isfile

_DATASET_CONFIG_FILE="dataset.conf"

def dataset_cropFaces(dsFolder, faceDConfig, config):
  """
      Crop faces in dataset
  """
  print "INFO: start cropping faces with 'facecrop'"
  for c in config['CLASSES']:
    ipath=join(dsFolder, join( config['IMAGESFOLDER'],c))
    opath=join(dsFolder, join( config['FACESFOLDER'],c))
    imgs=[ f for f in os.listdir(ipath) if isfile(join(ipath,f))]
    for im in imgs:
      iimPath=join(ipath,im)
      oimPath=join(opath,im)
      print "INFO: running facecropping on %s " % im
      retcode=subprocess.call(["./facecrop_cli", str(faceDConfig), str(iimPath), str(oimPath), "--register"])
      if retcode<0:
        print "WARN: execution has returned error %d" % retcode

if __name__ == "__main__":
  parser = argparse.ArgumentParser()
  parser.add_argument("datasetFolder", help="Dataset folder")
  parser.add_argument("faceDetectorCfg", help="Face Detector configuration file (OpenCV XML)")
  args = parser.parse_args()
  config={}
  configFile=join(args.datasetFolder, _DATASET_CONFIG_FILE)
  if not os.path.exists(configFile):
    print "ERR: dataset configuration file '%s' not found" % _DATASET_CONFIG_FILE
    exit(-1)
  print "INFO: Reading configuration file at '%s'" % configFile
  execfile(configFile, config)

  dataset_cropFaces(args.datasetFolder, args.faceDetectorCfg, config)
