#!/usr/bin/env python2

import os
import shutil
import argparse

from string import strip
from os.path import join
from os.path import isfile
from os.path import isdir

_DATASET_CONFIG_FILE="dataset.conf"
CONFIG={}

def filldatasetCohnKanade(dsFolder,ckFolder,ckEmoFolder):
	""" """
	global CONFIG
	configFile=join(dsFolder,_DATASET_CONFIG_FILE)
	if not os.path.exists(configFile):
		print "ERR: dataset configuration file '%s' not found" % _DATASET_CONFIG_FILE
		return
	if VERBOSE:
		print "INFO: Reading configuration file at '%s' " %configFile
	execfile(configFile, CONFIG)

	subjects=[ x for x in os.listdir(ckFolder) if isdir(join(ckFolder,x)) ]

	if VERBOSE:
		print "INFO: %d subjects found in CK+ database" % len(subjects)

	for subj in subjects:
		if VERBOSE:
			print "INFO: Processing subject %s " % subj
		labelFolders=[x for x in os.listdir(join(ckEmoFolder,subj)) if isdir(join(ckEmoFolder,join(subj,x)))]
		imageFolders=[x for x in os.listdir(join(ckFolder,subj)) if isdir(join(ckEmoFolder,join(subj,x)))]

		shots=[x for x in imageFolders if x in labelFolders]

		for s in shots:
			if VERBOSE:
				print "INFO: Processing shot %s " % s
			pics=[x for x in os.listdir( join(ckFolder,join(subj,s)) ) if isfile(join(ckFolder,join(subj,join(s,x))))]
			pics.sort()
			labels=[x for x in os.listdir( join(ckEmoFolder,join(subj,s)) ) if isfile( join(ckEmoFolder,join(subj,join(s,x)) )) ]
			if len(labels)<1 or len(pics)<1:
				# label forlder could contain no file at all, in this case skip the current shot
				if VERBOSE:
					"WARNING: subject %s shot %s has #%d emo labels and #%d pictures" %( subj,s,len(labels),len(pics))
				continue
			emo=None
			with open(join(ckEmoFolder,join(subj,join(s,labels[0]))) ,"r") as f:
				buf=f.read()
				if len(buf)==0:
					if VERBOSE:
						"WARNING: subject %s shot %s has void emo label '%s'" % (subj,s,join(ckEmoFolder,join(subj,join(s,labels[0]))))
					# A label file could be void, in this case skip the current shot
					continue
				try:
					emo=CONFIG['CLASSES'][int(float(strip(buf)))]
				except:
					if VERBOSE:
						print "ERR: cannot parse emotional label for subject %s shot %s " % (subj,s)
						continue
			pic=pics[-1]
			if VERBOSE:
				print "INFO: Picture '%s' has been marked as %s" % (pic,emo)
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
	parser.add_argument("-v","--verbose",action='store_true',help="verbose")
	args=parser.parse_args()
	if args.verbose:
		VERBOSE=True
	else:
		VERBOSE=False
	filldatasetCohnKanade(args.datasetFolder,args.cohnKanadeFolder,args.cohnKanadeEmotionsFolder)
