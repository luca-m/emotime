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
    #comstr=' '.join(param)
    if retcode==0:
      #print "INFO: done %s"%comstr
      return (comstr,True)
    else:
      print "ERR: '%s' has encountered problems" % comstr 
      return (comstr,False)

def dataset_calcGaborBank(dsFolder, config):
  """ 
      Calculate features using a gabor filters bank 
  """
  bagoftask = []

  for c in config['CLASSES']:
    facesFolder=join(dsFolder, join(config['FACES_FOLDER'], c))
    featsFolder=join(dsFolder, join(config['FEATURES_FOLDER'], c))
    faces=[ f for f in os.listdir(facesFolder) if isfile(join(facesFolder, f))]
    
    for i in xrange(0, len(faces)):
      face = faces[i]
      faceFile=join(facesFolder, face)
      #sys.stdout.write("INFO: calculating features for %s (%d of %d)\r" % (c,(i+1), len(faces)))
      #sys.stdout.flush()
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

def dataset_calcFeatures(dsFolder, config):
  """ Calculate features on dataset"""
  print "INFO: calculating gabor features"
  dataset_calcGaborBank(dsFolder,config)

if __name__ == "__main__":
  parser = argparse.ArgumentParser()
  parser.add_argument("--cfg", default="dataset.cfg", help="Dataset config file name")
  parser.add_argument("dsFolder", help="Dataset base folder")
  args = parser.parse_args()
  try:
    config={}
    config=dcp.parse_ini_config(join(args.dsFolder, args.cfg))
    dataset_calcFeatures(args.dsFolder, config)
  except Exception as e:
    print "ERR: something wrong (%s)" % str(e)
    sys.exit(1)


