#!/usr/bin/env python2
#import sys
import os
import subprocess
import argparse

from os.path import join
from os.path import isfile

_DATASET_CONFIG_FILE="dataset.conf"
CONFIG={}


def cropFaces(dsFolder,faceDConfig):
	global CONFIG
	configFile=join(dsFolder,_DATASET_CONFIG_FILE)
	if not os.path.exists(configFile):
		print "ERR: dataset configuration file '%s' not found" % _DATASET_CONFIG_FILE
		return
	if VERBOSE:
		print "INFO: Reading configuration file at '%s' " %configFile
	execfile(configFile, CONFIG)

	if not os.path.exists(faceDConfig):
		if VERBOSE:
			print "ERR: Face detection configuration file not found"
		return


	for c in CONFIG['CLASSES']:
		ipath=join(dsFolder, join( CONFIG['IMAGESFOLDER'],c))
		opath=join(dsFolder, join( CONFIG['FACESFOLDER'],c))
		imgs=[ f for f in os.listdir(ipath) if isfile(join(ipath,f))]
		for im in imgs:
			iimPath=join(ipath,im)
			oimPath=join(opath,im)
			if VERBOSE:
				print "INFO: running %s " % ' '.join(["./facecrop",str(faceDConfig),str(iimPath),str(oimPath),"--register"])

			retcode=subprocess.call(["./facecrop",str(faceDConfig),str(iimPath),str(oimPath),"--register"])

			if retcode<0:
				if VERBOSE:
					print "WARNING: execution has returned error %d " % retcode

if __name__ == "__main__":
	global VERBOSE
	parser = argparse.ArgumentParser()
	parser.add_argument("datasetFolder",help="Dataset folder")
	parser.add_argument("faceDetectorCfg",help="Face Detector configuration file (OpenCV XML)")
	parser.add_argument("-v","--verbose",action='store_true',help="verbosity")
	args = parser.parse_args()
	if args.verbose:
		VERBOSE=True
	else:
		VERBOSE=False
	cropFaces(args.datasetFolder,args.faceDetectorCfg)
