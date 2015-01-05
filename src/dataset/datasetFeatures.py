#!/usr/bin/env python2
"""
  Calculate Gabor features features 
"""
import os
import argparse
import subprocess
import datasetConfigParser as dcp
import sys
import multiprocessing

from os.path import join
from os.path import isfile

def _subproc_call(args):
    """ Wrap a subprocess.call """
    param, comstr = args
    retcode=subprocess.call( param, shell=False )
    if retcode==0:
      return (comstr,True)
    else:
      print("ERR: '%s' has encountered problems" % comstr) 
      return (comstr,False)

def dataset_calcGaborBank(dsFolder, config, validation=False):
  """ 
      Calculate features using a gabor filters bank 
  """
  bagoftask = []
  
  FACES_FOLDER = config['TRAINING_FACES']
  FEATURES_FOLDER = config['TRAINING_FEATURES']
  if validation:
    FACES_FOLDER = config['VALIDATION_FACES']
    FEATURES_FOLDER = config['VALIDATION_FEATURES']

  for c in config['CLASSES']:
    facesFolder=join(dsFolder, join(FACES_FOLDER, c))
    featsFolder=join(dsFolder, join(FEATURES_FOLDER, c))
    faces=[ f for f in os.listdir(facesFolder) if isfile(join(facesFolder, f))]
    
    for i in xrange(0, len(faces)):
      face = faces[i]
      faceFile=join(facesFolder, face)
      featFolder=join(featsFolder, os.path.splitext(face)[0]) + config['FILTERED_FOLDER_SUFFIX']
      try:
        os.mkdir(featFolder)
      except Exception:
        pass

      featFile=join(featFolder, config['GABOR_FEAT_FNAME'])
      cmd=[config['GABOR_TOOL'], str(config['SIZE']['width']), str(config['SIZE']['height']), 
           config['GABOR_NWIDTHS'], config['GABOR_NLAMBDAS'], config['GABOR_NTHETAS'], 
           str(faceFile), str(featFile)]
      if 'GABOR_FILTER_FILE' in config.keys():
        if config['GABOR_FILTER_FILE'] != 'NA':
          cmd.append(config['GABOR_FILTER_FILE'])
      
      bagoftask.append((cmd,'GaborFileter {0}'.format(faceFile)))

    # Spawining parallel task
    nprocs = max(1, int(multiprocessing.cpu_count() * abs(float(config['TRAIN_SVM_CPU_USAGE']))))
    results = []
    pool = multiprocessing.Pool(processes= nprocs)
    pool.map_async(_subproc_call, bagoftask, callback = results.append).get(2**32) # workaround for properly handling SIGINT
    pool.close()
    pool.join()

def dataset_calcFeatures(dsFolder, config, validation=False):
  """ Calculate features on dataset"""
  print("INFO: calculating gabor features")
  dataset_calcGaborBank(dsFolder,config, validation=validation)

if __name__ == "__main__":
  parser = argparse.ArgumentParser()
  parser.add_argument("--cfg", default="dataset.cfg", help="Dataset config file name")
  parser.add_argument("--validation", action="store_true", help="true if it is validation")
  parser.add_argument("dsFolder", help="Dataset base folder")
  args = parser.parse_args()
  try:
    config={}
    config=dcp.parse_ini_config(args.cfg)
    dataset_calcFeatures(args.dsFolder, config, validation=args.validation)
  except Exception as e:
    print("ERR: something wrong (%s)" % str(e))
    sys.exit(1)


