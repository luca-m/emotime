#!/usr/bin/env python2
import os
import subprocess
import argparse
import datasetConfigParser as dcp

from os.path import join
from os.path import isfile

def dataset_cropFaces(dsFolder, config):
  """
      Crop faces in dataset
  """
  print "INFO: start cropping faces with 'facecrop'"
  for c in config['CLASSES']:
    ipath=join(dsFolder, join( config['IMAGES_FOLDER'], c))
    opath=join(dsFolder, join( config['FACES_FOLDER'], c))
    imgs=[ f for f in os.listdir(ipath) if isfile(join(ipath, f))]
    for im in imgs:
      iimPath=join(ipath, im)
      oimPath=join(opath, im)
      print "INFO: running facecropping on %s " % im
      retcode=subprocess.call([ config['FACECROP_TOOL'], config['FACECROP_FACE_DETECTOR_CFG'], config['FACECROP_EYE_DETECTOR_CFG'], str(iimPath), str(oimPath)])
      if retcode < 0:
        print "WARN: execution has returned error %d" % retcode

if __name__ == "__main__":
  parser = argparse.ArgumentParser()
  parser.add_argument("--cfg", default="dataset.cfg", help="Dataset config file name")
  parser.add_argument("dsFolder", help="Dataset base folder")
  args = parser.parse_args()
  try:
    config={}
    config=dcp.parse_ini_config(join(args.dsFolder, args.cfg))
    dataset_cropFaces(args.dsFolder, config)
  except Exception as e:
    print "ERR: something wrong (%s)" % str(e)

