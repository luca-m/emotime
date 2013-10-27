#!/usr/bin/env python2
"""
    
"""
import argparse
import os
import subprocess
import multiprocessing
import string
from os.path import join
from os.path import isfile
from os.path import isdir
from os.path import basename
from os.path import splitext


_DATASET_CONFIG_FILE="dataset.conf"
_FILTER_FILE_EXT=".dat"
_USAGE=.5

def _subproc_call( param ):
    """ Wrap a subprocess.call """
    retcode=subprocess.call( param, shell=False )
    comstr=' '.join(param)
    if retcode==0:
      print "INFO: '%s' has completed successfully " % comstr
      return (comstr,True)
    else:
      print "WARN: '%s' has encountered problems" % comstr 
      return (comstr,False)

def dataset_selectFeatures(classifierFolder, filterFile):
  """
      Prepare the feature filter using trained boost trees
  """
  print "INFO: starting feature selection."
  for f in os.listdir(classifierFolder):
    if string.lower(splitext(f)[1]) == ".xml":
      retcode=subprocess.call( ["./featselect_cli", join(classifierFolder,f), join(classifierFolder,filterFile) ] )
      if retcode<0:
        print "WARN: extracting selected features for '%s' has returned error (%d) " % (f,retcode)

  print "INFO: selected feature index written to %s" % (filterFile)

def dataset_trainAdaboost(trainFolder, outFolder):
  """
      Train adaboos classifiers
  """
  bagoftask=[]
  print "INFO: starting adaboost training"
  for f in os.listdir(trainFolder):
    ext=os.path.splitext(f)
    of=f[:-len(ext)]+'.xml'
    fields=0
    # Detect number of features
    with open(join(trainFolder,f),'r') as r:
      fields=len(r.readline().split(','))
    bagoftask.append( ["./adatrain_cli", join(trainFolder,f), join(outFolder,of), str(fields), '-p' ] ) 
  print "INFO: bag of task prepared. Start training.."
  nprocs=min( 1, int(multiprocessing.cpu_count()*abs(_USAGE)), multiprocessing.cpu_count() )
  results=[]
  pool=multiprocessing.Pool(processes=nprocs)
  res=pool.map_async(_subproc_call,bagoftask,callback=results.append)
  res.wait()
  print "INFO: AdaBoost training finished."
  return results

def run_training(dsFolder, config):
  """
      Start training
  """
  classifFldr=join(dsFolder,config['CLASSIFIERFOLDER']) 
  trainFldr=join(dsFolder,config['TRAINFOLDER']) 
  print "INFO: training decision trees using AdaBoost (REAL).."
  results=dataset_trainAdaboost( trainFldr, classifFldr )
  print "INFO: selectiong features using trained boosted decision trees.."
  dataset_selectFeatures( classifFldr , join(classifFldr,"adaboost_featselection"+_FILTER_FILE_EXT))

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

