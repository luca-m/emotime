#!/usr/bin/env python2
"""
   Train with Adaboost and select relevant features 
"""
import argparse
import sys
import os
import subprocess
import multiprocessing
import datasetConfigParser as dcp

from os.path import join

def _subproc_call(args):
    """ Wrap a subprocess.call """
    param, comstr = args
    retcode=subprocess.call( param, shell=False )
    #comstr=' '.join(param)
    if retcode==0:
      print "INFO: done %s"%comstr
      return (comstr,True)
    else:
      print "ERR: '%s' has encountered problems" % comstr 
      return (comstr,False)

def dataset_train(smode, trainFolder, outFolder, config):
  """
      Train svm classifiers
  """
  bagoftask = []

  print "INFO: starting svm training"
  for f in os.listdir(trainFolder):
    of = os.path.splitext(f)[0] + '.xml'
    bagoftask.append(([config['TRAIN_TOOL'], smode, '{0}'.format(join(trainFolder,
      f)), '{0}'.format(join(outFolder, of))], os.path.splitext(f)[0]))

  #print "INFO: tasks prepared, starting training procedure"

  nprocs = max(1, int(multiprocessing.cpu_count() * abs(float(config['TRAIN_SVM_CPU_USAGE']))))
  results = []
  pool = multiprocessing.Pool(processes= nprocs)
  pool.map_async(_subproc_call, bagoftask, callback =
      results.append).get(2**32) # workaround for properly handling SIGINT
  pool.close()
  pool.join()

  print "INFO: %s training finished."%smode
  return results

def dataset_run_training(dsFolder, config, mode):
  """
      Start training
  """
  trainFldr = join(dsFolder, config['TRAIN_FOLDER'])

  if mode == "adaboost":
    smode = "ada"
    classifFldr = join(dsFolder, config['CLASSIFIER_FOLDER'])
  else:
    smode = "svm"
    classifFldr = join(dsFolder, config['CLASSIFIER_SVM_FOLDER'])

  dataset_train(smode, trainFldr, classifFldr, config)

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
    sys.exit(1)

