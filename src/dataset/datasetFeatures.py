#!/usr/bin/env python2
"""
  Calculate Gabor features features 
"""
import os
import argparse
import subprocess
import datasetConfigParser as dcp

from os.path import join
from os.path import isfile


def dataset_calcGaborBank(dsFolder, config):
  """ 
      Calculate features using a gabor filters bank 
  """
  for c in config['CLASSES']:
    facesFolder=join(dsFolder, join(config['FACES_FOLDER'], c))
    featsFolder=join(dsFolder, join(config['FEATURES_FOLDER'], c))
    faces=[ f for f in os.listdir(facesFolder) if isfile(join(facesFolder, f))]
    for face in faces:
      faceFile=join(facesFolder, face)
      print "INFO: calculating features on %s" % faceFile
      featFolder=join(featsFolder, os.path.splitext(face)[0]) + config['FILTERED_FOLDER_SUFFIX']
      try:
        os.mkdir(featFolder)
      except Exception as e:
        print "WARN: creation of directory failed (%s)" % str(e)
        pass
      featFile=join(featFolder, config['GABOR_FEAT_FNAME'])
      cmd=[config['GABOR_TOOL'], str(config['SIZE']['width']), str(config['SIZE']['height']), 
           config['GABOR_NWIDTHS'], config['GABOR_NLAMBDAS'], config['GABOR_NTHETAS'], 
           str(faceFile), str(featFile)]
      if 'GABOR_FILTER_FILE' in config.keys():
        if config['GABOR_FILTER_FILE'] != 'NA':
          cmd.append(config['GABOR_FILTER_FILE'])
      retcode=subprocess.call(cmd)
      if retcode<0:
        print "WARN: execution has returned error %d" % retcode

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


