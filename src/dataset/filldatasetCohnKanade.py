#!/usr/bin/env python2

import os
import shutil
import argparse

from string import strip
from os.path import join
from os.path import isfile
from os.path import isdir

_DATASET_CONFIG_FILE="dataset.conf"
VERBOSE=False
CONFIG={}

def filldatasetCohnKanade(dsFolder,ckFolder,ckEmoFolder):
	""" """
	global CONFIG
	configFile=join(dsFolder,_DATASET_CONFIG_FILE)
	if not os.path.exist(configFile):
		print "ERR: dataset configuration file '%s' not found" % _DATASET_CONFIG_FILE
		return
	if VERBOSE:
		print "INFO: Reading configuration file at '%s' " %configFile
	execfile(configFile, CONFIG)

	subjects=[ x for x in os.listdir(ckFolder) if isdir(x) ]

	for subj in subjects:
		labelFolders=[x for x in os.listdir(join(ckEmoFolder,subj)) if isdir(x)]
		imageFolders=[x for x in os.listdir(join(ckFolder,subj)) if isdir(x)]

		shots=[x for x in imageFolders if x in labelFolders]

		for s in shots:
			pics=[x for x in os.listdir( join(ckFolder,join(subj,s)) ) if isfile(x)]
			pics.sort()
			labels=[x for x in os.listdir( join(ckEmoFolder,join(subj,s)) ) if isfile(x)]

			if len(labels)==0 or len(pics)==0:
				# label forlder could contain no file at all, in this case skip the current shot
				continue
			emo=None
			with open(join(ckEmoFolder,join(subj,join(s,labels[0]))) ,"r") as f:
				buf=f.read()
				if len(buf)==0:
					# A label file could be void, in this case skip the current shot
					continue
				try:
					emo=CONFIG['CLASSES'][int(float(strip(buf)))]
				except:
					if VERBOSE:
						print "ERR: cannot parse emotional label for subject %s shot %s " % (subj,s)
						continue
			pic=pics[0]
			orig=join(ckFolder, join(subj, join(s, pic)))
			dest= join(dsFolder, join(CONFIG['IMAGESFOLDER'], join(emo, pic)))
			try:
				shutil.copy(orig, dest)
			except:
				if VERBOSE:
					print "ERR: cannot copy image '%s' to dataset '%s' "%(orig,dest)
					continue

if __name__ == "__main__":
	global VERBOSE
	parser=argparse.ArgumentParser()
	parser.add_argument("datasetFolder",help="Dataset folder")
	parser.add_argument("cohnKanadeFolder",help="Cohn-Kanade image database folder")
	parser.add_argument("cohnKanadeEmotionsFolder",help="Cohn-Kanade (CK+) emotion label folder")
	parser.add_argument("--verbose",help="increase output verbosity")
	args=parser.parse_args()
	if args.verbose:
		VERBOSE=True
	filldatasetCohnKanade(args.datasetFolder,args.cohnKanadeFolder,args.cohnKanadeEmotionsFolder)
