#!/usr/bin/env python2
import os
import argparse
import subprocess

from os.path import join
from os.path import isfile

_DATASET_CONFIG_FILE="dataset.conf"

def dataset_calcGaborBank(dsFolder,config):
  """ 
      Calculate features using a gabor filters bank 
  """
  print "INFO: calculating gabor bank features"
  for c in config['CLASSES']:
    ipath=join(dsFolder, join( config['FACESFOLDER'],c))
    opath=join(dsFolder, join( config['FEATURESFOLDER'],c))
    imgs=[ f for f in os.listdir(ipath) if isfile(join(ipath,f))]
    for im in imgs:
      iimPath=join(ipath,im)
      print "INFO: calculating features on  %s " % iimPath
      oPath=join(opath,os.path.splitext(im)[0] ) + config['FILTERED_FOLDER_SUFFIX']
      try:
        os.mkdir(oPath)
      except:
        print "WARN: directory '%s' already exist." % oPath
        pass
      retcode=subprocess.call(["./gaborbank_cli",str(config['SIZE']['width']),str(config['SIZE']['height']),str(iimPath),str(oPath)])
      if retcode<0:
        print "WARN: execution has returned error %d " % retcode

def dataset_calcFeatures( dsFolder, config):
  """ Calculate features on dataset"""
  print "INFO: calculating face features"
  dataset_calcGaborBank(dsFolder,config)


if __name__ == "__main__":
  parser = argparse.ArgumentParser()
  parser.add_argument("datasetFolder",help="Dataset folder")
  args = parser.parse_args()
  config={}
  configFile=join(args.datasetFolder,_DATASET_CONFIG_FILE)
  if not os.path.exists(configFile):
    print "ERR: dataset configuration file '%s' not found" % _DATASET_CONFIG_FILE
    exit(-1)
  print "INFO: Reading configuration file at '%s' " %configFile
  execfile(configFile, config)
  dataset_calcFeatures(args.datasetFolder,config)
