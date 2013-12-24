#!/usr/bin/env python2
"""
   Train with Adaboost and select relevant features 
"""
import argparse
import os
import subprocess
import multiprocessing
import string
import datasetConfigParser as dcp

from os.path import join
from os.path import isfile
from os.path import isdir
from os.path import basename
from os.path import splitext

def _subproc_call( param ):
    """ Wrap a subprocess.call """
    retcode=subprocess.call( param, shell=False )
    comstr=' '.join(param)
    if retcode==0:
      print "INFO: '%s' has completed successfully " % comstr
      return (comstr,True)
    else:
      print "ERR: '%s' has encountered problems" % comstr 
      return (comstr,False)

def dataset_selectFeatures(classifierFolder, filterFile, config):
  """
      Prepare the feature filter using trained boost trees
  """
  print "INFO: starting feature selection."
  for f in os.listdir(classifierFolder):
    if string.lower(splitext(f)[1]) == ".xml":
      retcode=subprocess.call( [config['TRAIN_FEATSEL_TOOL'], '{0}'.format(join(classifierFolder, f)), '{0}'.format(join(classifierFolder, filterFile)) ] )
      if retcode<0:
        print "WARN: extracting selected features for '%s' has returned error (%d)" % (f, retcode)
  print "INFO: selected feature index written to %s" % (filterFile)

def dataset_trainAdaboost(trainFolder, outFolder, config):
  """
      Train adaboos classifiers
  """
  bagoftask=[]
  print "INFO: starting adaboost training"
  for f in os.listdir(trainFolder):
    ext=os.path.splitext(f)[1]
    of=f[:-len(ext)]+'.xml'
    
    #fields=0
    # Detect number of features
    #with open(join(trainFolder,f),'r') as r:
    #  fields=len(r.readline().split(','))

    bagoftask.append( [config['TRAIN_ADA_TOOL'], '{0}'.format(join(trainFolder, f)), '{0}'.format(join(outFolder, of)) ])#, str(fields)] ) 
  
  print "INFO: tasks prepared, starting training procedure"
  nprocs=max( 1, int(multiprocessing.cpu_count()*abs(float(config['TRAIN_ADA_CPU_USAGE']))) )
  results=[]
  pool=multiprocessing.Pool(processes=nprocs)
  res=pool.map_async(_subproc_call,bagoftask,callback=results.append).get(2**32) # workaround for properly handling SIGINT
  #res.wait()
  print "INFO: AdaBoost training finished."
  return results

def dataset_trainSVM(trainFolder, outFolder, config):
  """
      Train svm classifiers
  """
  bagoftask = []

  print "INFO: starting svm training"
  for f in os.listdir(trainFolder):
    ext = os.path.splitext(f)[1]
    of = 'svm_' + f[:-len(ext)] + '.xml'
    bagoftask.append([config['TRAIN_SVM_TOOL'], '{0}'.format(join(trainFolder,
      f)), '{0}'.format(join(outFolder, of)) ])

  print "INFO: tasks prepared, starting training procedure"

  nprocs = max(1, int(multiprocessing.cpu_count() * abs(float(config['TRAIN_SVM_CPU_USAGE']))))
  results = []
  pool = multiprocessing.Pool(processes= nprocs)
  res = pool.map_async(_subproc_call, bagoftask, callback=
      results.append).get(2**32) # workaround for properly handling SIGINT
  res.wait()

  print "INFO: SVM training finished."
  return results

def dataset_run_training(dsFolder, config, mode):
  """
      Start training
  """
  classifFldr=join(dsFolder, config['CLASSIFIER_FOLDER']) 
  trainFldr=join(dsFolder, config['TRAIN_FOLDER']) 

  if mode == "adaboost":
    print "INFO: training decision trees using AdaBoost"
    results=dataset_trainAdaboost(trainFldr, classifFldr, config)
    classif_file = config['TRAIN_ADA_FILTER_FNAME']
    print "INFO: selectiong features using trained decision trees"
    dataset_selectFeatures(classifFldr, join(classifFldr, classif_file), config)

  elif mode == "svm":
    print "INFO: training decision trees using SVM"
    results = dataset_trainSVM(trainFldr, classifFldr, config)

if __name__ == "__main__":
  parser = argparse.ArgumentParser()
  parser.add_argument("--cfg", default="dataset.cfg", help="Dataset config file name")
  parser.add_argument("dsFolder", help="Dataset base folder")
  parser.add_argument("-v", "--verbose", action='store_true', help="verbosity")
  parser.add_argument("--mode", default="adaboost", choices=['adaboost', 'svm'], help="training mode: adaboost or svm")
  args = parser.parse_args()

  try:
    config={}
    config=dcp.parse_ini_config(join(args.dsFolder, args.cfg))
    dataset_run_training(args.dsFolder, config, args.mode)
  except Exception as e:
    print "ERR: something wrong (%s)" % str(e)


