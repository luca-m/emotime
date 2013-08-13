#!/usr/bin/env python2
import os
import argparse
import subprocess

from os.path import join
from os.path import isfile

_DATASET_CONFIG_FILE="dataset.conf"
CONFIG={}

def calculateFeatures(dsFolder):
	""" """
	global CONFIG
	configFile=join(dsFolder,_DATASET_CONFIG_FILE)
	if not os.path.exists(configFile):
		print "ERR: dataset configuration file '%s' not found" % _DATASET_CONFIG_FILE
		return
	if VERBOSE:
		print "INFO: Reading configuration file at '%s' " %configFile
	execfile(configFile, CONFIG)
	
	for c in CONFIG['CLASSES']:
		ipath=join(dsFolder, join( CONFIG['FACESFOLDER'],c))
		opath=join(dsFolder, join( CONFIG['FEATURESFOLDER'],c))
		imgs=[ f for f in os.listdir(ipath) if isfile(join(ipath,f))]
		for im in imgs:
			iimPath=join(ipath,im)
			oPath=join(opath,os.path.splitext(im)[0])
			try:
				os.mkdir(oPath)
			except:
				# already exist
				pass

			if VERBOSE:
				print "INFO: running %s " % ' '.join(["./gaborbank",str(CONFIG['SIZE']['width']),str(CONFIG['SIZE']['height']),str(iimPath),str(oPath)])

			retcode=subprocess.call(["./gaborbank",str(CONFIG['SIZE']['width']),str(CONFIG['SIZE']['height']),str(iimPath),str(oPath)])

			if retcode<0:
				if VERBOSE:
					print "WARNING: execution has returned error %d " % retcode



if __name__ == "__main__":
	global VERBOSE
	parser = argparse.ArgumentParser()
	parser.add_argument("datasetFolder",help="Dataset folder")
	parser.add_argument("-v","--verbose",action='store_true',help="verbosity")
	args = parser.parse_args()
	if args.verbose:
		VERBOSE=True
	else:
		VERBOSE=False
	calculateFeatures(args.datasetFolder)