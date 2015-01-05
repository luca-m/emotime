#!/usr/bin/env python2
"""
  Import the Chon-Kanade plus database to a dataset.
"""
import os
import sys
import shutil
import argparse
import datasetConfigParser as dcp
import random

from string import strip
from os.path import join
from os.path import isfile
from os.path import isdir


def dataset_fillCohnKanade( dsFolder, ckFolder, ckEmoFolder, config, vperc=0.3, vseed=0):
  """ 
      Fill dataset with Cohn Kanade + data.
  """

  subjects=[ x for x in os.listdir(ckFolder) if isdir(join(ckFolder, x)) ]
  print "INFO: %d subjects found in CK+ database" % len(subjects)

  for subj in subjects:
    print "INFO: Processing subject %s " % subj
    
    labelFolders=[x for x in os.listdir(join(ckEmoFolder, subj)) if isdir(join(ckEmoFolder, join(subj, x)))]
    imageFolders=[x for x in os.listdir(join(ckFolder, subj)) if isdir(join(ckEmoFolder, join(subj, x)))]

    shots=[x for x in imageFolders if x in labelFolders]
    for s in shots:
      print "INFO: Processing shot %s " % s
      
      pics=[x for x in os.listdir( join(ckFolder, join(subj,s)) ) if isfile(join(ckFolder, join(subj, join(s, x))))]
      pics.sort()
      labels=[x for x in os.listdir( join(ckEmoFolder, join(subj, s)) ) if isfile( join(ckEmoFolder, join(subj, join(s, x)) )) ]
      if len(labels)<1 or len(pics)<1:
        # label forlder could contain no file at all, in this case skip the current shot or mark it as neutral?
        print "WARN: subject %s shot %s has #%d emo labels and #%d pictures, (skip:incomplete)" %( subj, s, len(labels), len(pics))
        continue
      emo=None
      with open(join(ckEmoFolder, join(subj, join(s, labels[0]))), "r") as f:
        buf=f.read()
        if len(buf)==0:
          print "WARN: subject %s shot %s has void emo label '%s', (skip:noemo)" % (subj, s, join(ckEmoFolder, join(subj, join(s, labels[0]))))
          # A label file could be void, in this case skip the current shot
          continue
        try:
          emo=config['CLASSES'][int(float(strip(buf)))]
        except:
          print "ERR: cannot parse emotional label for subject %s shot %s (skip:unknown_emo)" % (subj, s)
          continue

      # Last picture is the final emotion (most intense), first picture is neutral
      to_copy = [(pics[-1], emo), (pics[0], config['CLASSES'][0])]

      for pic, emo in to_copy:
        print "INFO: Picture '%s' has been marked as %s" % (pic, emo)
        orig = join(ckFolder, join(subj, join(s, pic)))
        IMAGES_FOLDER = config['TRAINING_IMAGES']
        if random.next() <= vperc:
            IMAGES_FOLDER = config['VALIDATION_IMAGES']
        dest = join(dsFolder, join(IMAGES_FOLDER, join(emo, pic)))
        try:
          shutil.copy(orig, dest)
        except:
          print "ERR: cannot copy image '%s' to dataset '%s' "%(orig, dest)
          continue

if __name__ == "__main__":
  parser=argparse.ArgumentParser()
  parser.add_argument("--cfg", default="dataset.cfg", help="Dataset config file name")
  parser.add_argument("--validation-perc",dest='vperc',type=float, default=0.3, help="Validation set percentage (0-1.0)")
  parser.add_argument("--validation-seed",dest='vseed',type=int, default=0, help="Seed used to decide when a image belong to training or validation set")
  parser.add_argument("dsFolder", help="Dataset Base folder (will be filled with images)")
  parser.add_argument("ckFolder", help="Cohn-Kanade image database folder")
  parser.add_argument("ckEmoFolder", help="Cohn-Kanade (CK+) emotion label folder")
  args=parser.parse_args()
  try:
    if args.vperc < 0. or args.vperc > 1.:
      raise new Exception('validation percentage must be in range 0-1')
    random.seed(args.vseed)
    config={}
    config=dcp.parse_ini_config(args.cfg)
    dataset_fillCohnKanade(args.dsFolder, args.ckFolder, args.ckEmoFolder, config, vperc=args.vperc, vseed=args.vseed)
  except Exception as e:
    print "ERR: something wrong (%s)" % str(e)
    sys.exit(1)

