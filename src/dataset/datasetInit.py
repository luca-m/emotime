#!/usr/bin/env python2
"""
  
"""
import os
import argparse
from os.path import join

_DEFAULT_CONFIG_FNAME="dataset.conf"

def dataset_init(dsPath, config):
  """ 
      Initialize dataset 
  """
  for clazz in config['CLASSES']:
    pth = join(dsPath, join(config['IMAGESFOLDER'], clazz))
    if not os.path.exists(pth):
      os.makedirs(pth)
    pth = join(dsPath, join(config['FACESFOLDER'], clazz))
    if not os.path.exists(pth):
      os.makedirs(pth)
    pth=join(dsPath, join(config['FEATURESFOLDER'], clazz))
    if not os.path.exists(pth):
      os.makedirs(pth)
  pth=join(dsPath, config['TRAINFOLDER'])
  if not os.path.exists(pth):
    os.makedirs(pth)
  pth=join(dsPath, config['CLASSIFIERFOLDER'])
  if not os.path.exists(pth):
    os.makedirs(pth)
  # Copy configuration
  with open(dsConfig, "r") as conf:
    configuration = conf.read()
    with open(join(dsPath, _DEFAULT_CONFIG_FNAME), "w") as nconf:
      nconf.write(configuration)

if __name__ == "__main__":
  parser = argparse.ArgumentParser()
  parser.add_argument("dsPath", help="Dataset folder path")
  parser.add_argument("config", help="Configuration file")
  args = parser.parse_args()
  config={}
  configFile=join(args.datasetFolder, _DATASET_CONFIG_FILE)
  if not os.path.exists(configFile):
    print "ERR: dataset configuration file '%s' not found" % _DATASET_CONFIG_FILE
    exit(-1)
  print "INFO: Reading configuration file at '%s'" % configFile
  execfile(configFile, config)
  dataset_init(args.dsPath, config)

