#!/usr/bin/env python2
"""
  
"""
import os
import argparse
from os.path import join

_DEFAULT_CONFIG_FNAME="dataset.conf"

def dataset_init(dsPath, dsConfig):
  """ 
      Initialize dataset 
  """
  config={}
  execfile(dsConfig, config)
  for clazz in CONFIG['CLASSES']:
    pth = join(dsPath,join(config['IMAGESFOLDER'],clazz))
    if not os.path.exists(pth):
      os.makedirs(pth)
    pth = join(dsPath,join(config['FACESFOLDER'],clazz))
    if not os.path.exists(pth):
      os.makedirs(pth)
    pth=join(dsPath,join(config['FEATURESFOLDER'],clazz))
    if not os.path.exists(pth):
      os.makedirs(pth)
  pth=join(dsPath,config['TRAINFOLDER'])
  if not os.path.exists(pth):
    os.makedirs(pth)
  pth=join(dsPath,config['CLASSIFIERFOLDER'])
  if not os.path.exists(pth):
    os.makedirs(pth)
  # Copy configuration
  with open( dsConfig,"r") as conf:
    configuration = conf.read()
    with open(join(dsPath,_DEFAULT_CONFIG_FNAME),"w") as nconf:
      nconf.write(configuration)

if __name__ == "__main__":
  parser = argparse.ArgumentParser()
  parser.add_argument("dsPath",help="Dataset folder path")
  parser.add_argument("config", help="Configuration file")
  args = parser.parse_args()
  dataset_init(args.dsPath,args.config)

