#!/usr/bin/env python2
"""
   
"""
import cv2 
import os
import itertools
import argparse
import datasetConfigParser as dcp

from os.path import join
from os.path import isfile
from os.path import isdir
from os.path import splitext
from os.path import basename 

def _dataset_load_matrix(filepath):
  """ """
  name,ext=splitext(basename(filepath))
  if ext in ['.yml','.xml']:
    return cv2.cv.Load(filepath, name=name)
  else:
    return cv2.imread(filepath, cv2.CV_LOAD_IMAGE_GRAYSCALE)

def _dataset_multiclass1toAll(config):
  """ """
  for z in [ ([x],[y for y in config['CLASSES'] if y != x]) for x in config['CLASSES']]:
    yield z

def _dataset_multiclass1to1(config):
  """ """
  for x,y in itertools.combinations( config['CLASSES'] , 2 ):
    yield ([x],[y])

def dataset_prepare( (goodClass,badClass), dsFolder, config):
  """ 
     Prepare train file with positive and negative samples.
  """
  print "INFO: Preparing training file for '%s' vs. '%s' " % (','.join(goodClass), ','.join(badClass))

  badClass = sorted(badClass)
  goodClass = sorted(goodClass)

  goodPath=[join(dsFolder, join(config['FEATURES_FOLDER'], goodClass)) for x in goodClass]
  badPath= [join(dsFolder, join(config['FEATURES_FOLDER'], x)) for x in badClass]
  #
  # Note: a goodFolder should contain the filtered images (various orientation and frequency ) 
  #       of a single sample image. So each line of the training file will be composed by its
  #    MARKER ( Good or Bad ) plus all the pixel values of the filtered images 
  #
  goodFolders=[]
  for x in goodPath:
    goodFolders.exptend( [ join(goodPath, f) for f in os.listdir(goodPath) if isdir(join(goodPath, f)) and f.endswith(config['FILTERED_FOLDER_SUFFIX']) ] )
  goodFolders.sort()

  badFolders=[]
  for x in badPath:
    badFolders.extend( [ join(badPath, f) for f in os.listdir(badPath) if isdir(join(badPath, f)) and f.endswith(config['FILTERED_FOLDER_SUFFIX']) ] )
  badFolders.sort()

  outfpath=join( join(dsFolder, config['TRAIN_FOLDER']), "%s_vs_%s%s" % ( '_'.join(goodClass), '_'.join(badClass), config['FEATURE_FILE_SUFFIX']) )
  
  with open(outfpath, "w") as tf: 
    #
    # POSITIVE SAMPLES
    #
    for fold in goodFolders:
      goodImgs=[ f for f in os.listdir(fold) if isfile(join(fold, f))]
      goodImgs.sort()
      for f in goodImgs:
        print "INFO: Processing (P) '%s'" % join(fold, f)
        tf.write("P")     # POSITIVE
        img=_dataset_load_matrix(join(fold,f))
        if img is None:
          print "WARN: cannot open image %s, skip it" % join(fold, f)
          continue
        for j in xrange(img.shape[1]):
          for i in xrange(img.shape[0]):
            value = float(img.item(j, i))
            tf.write(",%d" % value)
      tf.write("\n")
    #
    # NEGATIVE SAMPLES
    #
    for fold in badFolders:
      badImgs=[f for f in os.listdir(fold) if isfile(join(fold, f))]
      badImgs.sort()
      for f in badImgs:
        print "INFO: Processing (N) '%s'" % join(fold, f)
        tf.write("N")     # NEGATIVE
        img=_dataset_load_matrix(join(fold, f))
        if img is None:
          print "WARN: cannot open image %s, skip it" % join(fold, f)
          continue
        for j in xrange(img.shape[1]):
          for i in xrange(img.shape[0]):
            value = float(img.item(j, i))
            tf.write(",%d" % value)
      tf.write("\n")
  print "INFO: Done"

def dataset_prepTrainFiles(dsFolder, config):
  """
      Prepare training files
  """
  #print "INFO: preparing training files for 1 to 1 multiclass"
  #for x in _dataset_multiclass1to1(config):
  #  dataset_prepare(x, dsFolder, config)
  
  print "INFO: preparing training files for 1 to All multiclass"
  for x in _dataset_multiclass1toAll(config):
    dataset_prepare(x, dsFolder, config)


if __name__ == "__main__":
  parser = argparse.ArgumentParser()
  parser.add_argument("--cfg", default="dataset.cfg", help="Dataset config file name")
  parser.add_argument("dsFolder",help="Dataset base folder")
  args = parser.parse_args()
  try:
    config={}
    config=dcp.parse_ini_config(join(args.dsFolder ,args.cfg))
    dataset_prepTrainFiles(args.datasetFolder, config)
  except Exception as e:
    print "ERR: something wrong (%s)" % str(e)
  
