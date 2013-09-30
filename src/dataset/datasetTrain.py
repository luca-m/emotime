#!/usr/bin/env python2
"""
    
"""
import argparse
import os
import subprocess
from os.path import join
from os.path import isfile
from os.path import isdir

_DATASET_CONFIG_FILE="dataset.conf"

def dataset_trainAdaboost(trainFolder, outFolder):
  """
      Train adaboos classifiers
  """
  print "INFO: starting adaboost training"
  for f in os.listdir(trainFolder):
    ext=os.path.splitext(f)
    of=f[:-len(ext)]+'.xml'
    fields=0
    # Detect number of features
    with open(join(trainFolder,f),'r') as r:
      fields=len(r.readline().split(','))
    # Train
    print "INFO: Training '%s' " % f
    retcode=subprocess.call( ["./adatrain", join(trainFolder,f), join(outFolder,of), str(fields), '-p' ] )
    if retcode<0:
      print "WARN: execution has returned error %d " % retcode
  print "INFO: adaboost training finished"

def run_training(dsFolder, config):
  """
      Start training
  """
  dataset_trainAdaboost( join(dsFolder,config['TRAINFOLDER']), join(dsFolder,config['CLASSIFIERFOLDER']) )

if __name__ == "__main__":
  parser = argparse.ArgumentParser()
  parser.add_argument("datasetFolder",help="Dataset folder")
  parser.add_argument("-v","--verbose",action='store_true',help="verbosity")
  args = parser.parse_args()
  config={}
  configFile=join(args.datasetFolder,_DATASET_CONFIG_FILE)
  if not os.path.exists(configFile):
    print "ERR: dataset configuration file '%s' not found" % _DATASET_CONFIG_FILE
    exit(-1)
  print "INFO: Reading configuration file at '%s' " %configFile
  execfile(configFile, config)
  run_training(args.datasetFolder, config)

