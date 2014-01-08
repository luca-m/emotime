#!/usr/bin/env python2
import os
import subprocess
import argparse
import datasetConfigParser as dcp

from os.path import join
from os.path import isfile

import sys

def dataset_cropFaces(dsFolder, config, eye_correction):
  """
      Crop faces in dataset
  """
  print "INFO: start cropping faces with 'facecrop'"

  if eye_correction:
    info_str = "with eye correction"
  else:
    info_str = "without eye correction"

  for c in config['CLASSES']:
    ipath=join(dsFolder, join( config['IMAGES_FOLDER'], c))
    opath=join(dsFolder, join( config['FACES_FOLDER'], c))
    imgs=[ f for f in os.listdir(ipath) if isfile(join(ipath, f))]
    for i in xrange(0, len(imgs)):
      im = imgs[i]
      iimPath = join(ipath, im)
      oimPath = join(opath, im)

      sys.stdout.write("INFO: running facecropping %s on %s (%d of %d)\r"%(info_str, c, (i + 1), len(imgs)))
      sys.stdout.flush()

      args = [config['FACECROP_TOOL'], config['FACECROP_FACE_DETECTOR_CFG']]
      if eye_correction:
        args.append(config['FACECROP_EYE_DETECTOR_CFG'])
      args.extend([str(iimPath), str(oimPath)])

      retcode = subprocess.call(args)
      if retcode < 0:
        print "\nWARN: execution has returned error %d" % retcode
    print ""

if __name__ == "__main__":
  parser = argparse.ArgumentParser()
  parser.add_argument("--cfg", default="dataset.cfg", help="Dataset config file name")
  parser.add_argument("--eye-correction", action="store_true", help="Apply eye correction to faces")
  parser.add_argument("dsFolder", help="Dataset base folder")
  args = parser.parse_args()
  try:
    config={}
    config=dcp.parse_ini_config(join(args.dsFolder, args.cfg))
    dataset_cropFaces(args.dsFolder, config, args.eye_correction)
  except Exception as e:
    print "ERR: something wrong (%s)" % str(e)

