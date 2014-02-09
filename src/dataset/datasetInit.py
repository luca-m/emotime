#!/usr/bin/env python2
"""
   Initialize a dataset  
"""
import os
import sys
import argparse
import datasetConfigParser as dcp

from os.path import join

def dataset_init(dsPath, config):
  """ 
      Initialize dataset 
  """
  for clazz in config['CLASSES']:
    pth = join(dsPath, join(config['IMAGES_FOLDER'], clazz))
    if not os.path.exists(pth):
      os.makedirs(pth)
    pth = join(dsPath, join(config['FACES_FOLDER'], clazz))
    if not os.path.exists(pth):
      os.makedirs(pth)
    pth=join(dsPath, join(config['FEATURES_FOLDER'], clazz))
    if not os.path.exists(pth):
      os.makedirs(pth)
  pth=join(dsPath, config['TRAIN_FOLDER'])
  if not os.path.exists(pth):
    os.makedirs(pth)
  pth=join(dsPath, config['CLASSIFIER_FOLDER'])
  if not os.path.exists(pth):
    os.makedirs(pth)
  pth=join(dsPath, config['CLASSIFIER_SVM_FOLDER'])
  if not os.path.exists(pth):
    os.makedirs(pth)
  # Copy configuration
  #with open(cfgFile, "r") as conf:
    #configuration = conf.read()
    #with open(join(dsPath, dsCfgName), "w") as nconf:
      #nconf.write(configuration)

if __name__ == "__main__":
  parser = argparse.ArgumentParser()
  parser.add_argument("--cfg", default="dataset.cfg", help="Dataset config file name")
  parser.add_argument("dsFolder", help="Dataset folder path")
  args = parser.parse_args()

  try:
    config={}
    config=dcp.parse_ini_config(args.cfg)
    dataset_init(args.dsFolder, config)
  except Exception as e:
    print "ERR: something wrong (%s)" % str(e)
    sys.exit(1)

