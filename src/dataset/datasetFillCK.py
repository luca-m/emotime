#!/usr/bin/env python2
"""

"""
import os
import shutil
import argparse

from string import strip
from os.path import join
from os.path import isfile
from os.path import isdir

_DATASET_CONFIG_FILE="dataset.conf"

def dataset_fillCohnKanade( dsFolder, ckFolder, ckEmoFolder, config):
  """ 
      Filla dataset with Cohn Kanade + data.
  """
  subjects=[ x for x in os.listdir(ckFolder) if isdir(join(ckFolder,x)) ]
  print "INFO: %d subjects found in CK+ database" % len(subjects)

  for subj in subjects:
    print "INFO: Processing subject %s " % subj
    labelFolders=[x for x in os.listdir(join(ckEmoFolder,subj)) if isdir(join(ckEmoFolder,join(subj,x)))]
    imageFolders=[x for x in os.listdir(join(ckFolder,subj)) if isdir(join(ckEmoFolder,join(subj,x)))]

    shots=[x for x in imageFolders if x in labelFolders]
    for s in shots:
      print "INFO: Processing shot %s " % s
      pics=[x for x in os.listdir( join(ckFolder,join(subj,s)) ) if isfile(join(ckFolder,join(subj,join(s,x))))]
      pics.sort()
      labels=[x for x in os.listdir( join(ckEmoFolder,join(subj,s)) ) if isfile( join(ckEmoFolder,join(subj,join(s,x)) )) ]
      if len(labels)<1 or len(pics)<1:
        # label forlder could contain no file at all, in this case skip the current shot
        print "WARN: subject %s shot %s has #%d emo labels and #%d pictures" %( subj,s,len(labels),len(pics))
        continue
      emo=None
      with open(join(ckEmoFolder,join(subj,join(s,labels[0]))) ,"r") as f:
        buf=f.read()
        if len(buf)==0:
          print "WARN: subject %s shot %s has void emo label '%s'" % (subj,s,join(ckEmoFolder,join(subj,join(s,labels[0]))))
          # A label file could be void, in this case skip the current shot
          continue
        try:
          emo=config['CLASSES'][int(float(strip(buf)))]
        except:
          print "ERR: cannot parse emotional label for subject %s shot %s " % (subj,s)
          continue
      pic=pics[-1]
      print "INFO: Picture '%s' has been marked as %s" % (pic,emo)
      orig=join(ckFolder, join(subj, join(s, pic)))
      dest= join(dsFolder, join(config['IMAGESFOLDER'], join(emo, pic)))
      try:
        shutil.copy(orig, dest)
      except:
        print "ERR: cannot copy image '%s' to dataset '%s' "%(orig,dest)
        continue

if __name__ == "__main__":
  parser=argparse.ArgumentParser()
  parser.add_argument("datasetFolder",help="Dataset folder")
  parser.add_argument("cohnKanadeFolder",help="Cohn-Kanade image database folder")
  parser.add_argument("cohnKanadeEmotionsFolder",help="Cohn-Kanade (CK+) emotion label folder")
  args=parser.parse_args()
  config={}
  configFile=join(args.datasetFolder,_DATASET_CONFIG_FILE)
  if not os.path.exists(configFile):
    print "ERR: dataset configuration file '%s' not found" % _DATASET_CONFIG_FILE
    exit(-1)
  print "INFO: Reading configuration file at '%s' " %configFile
  execfile(configFile, config)
  
  dataset_fillCohnKanade(args.datasetFolder,args.cohnKanadeFolder,args.cohnKanadeEmotionsFolder,config)

