#!/usr/bin/env python2

import cv2 as cv
import os
import itertools
import argparse

from os.path import join
from os.path import isfile
from os.path import isdir

_DATASET_CONFIG_FILE="dataset.conf"
CONFIG={}


def multiclassOnetoOne():
	""" """
	return [x for x in itertools.permutations( CONFIG['CLASSES'] ,2 )]

def prepareTrainingFile( (goodClass,badClass),  dsFolder ):
	""" """
	if VERBOSE:
		print "INFO: Preparing training file for '%s' vs. '%s' " %(goodClass,badClass)
	
	goodPath=join(dsFolder,join(CONFIG['FEATURESFOLDER'],goodClass))
	badPath=join(dsFolder,join(CONFIG['FEATURESFOLDER'],badClass))
	#
	# Note: a goodFolder should contain the filtered images (various orientation and frequency ) 
	#       of a single sample image. So each line of the training file will be composed by its
	#		MARKER ( Good or Bad ) plus all the pixel values of the filtered images 
	#
	goodFolders=[ join(goodPath,f) for f in os.listdir(goodPath) if isdir(join(goodPath,f)) and f.endswith(CONFIG['FILTERED_FOLDER_SUFFIX']) ]
	badFolders= [ join(badPath,f)  for f in os.listdir(badPath)  if isdir(join(badPath,f))  and f.endswith(CONFIG['FILTERED_FOLDER_SUFFIX']) ]

	outfpath=join(join(dsFolder,CONFIG['TRAINFOLDER']),"%s_%s%s"%(goodClass,badClass,CONFIG['FEATURE_FILE_SUFFIX']))
	with open( outfpath , "w") as tf:
		# Open gabor filtered images for each sample and prepare csv row
		for fold in goodFolders:
			goodImgs=[ f for f in os.listdir(fold) if isfile(join(fold,f))]
			goodImgs.sort()
			for f in goodImgs:
				if VERBOSE:
					print "INFO: Processing '%s'" % f 
				tf.write("P")
				img = cv.imread( f ,cv.CV_LOAD_IMAGE_GRAYSCALE)
				for i in xrange (img.shape[1]):
					for j in xrange(img.shape[0]):
						value = img.item(j, i)
						tf.write(",%d"%value)
			tf.write("\n")
		for fold in badFolders:
			badImgs=[f for f in os.listdir(fold) if isfile(join(fold,f))]
			badImgs.sort()
			for f in badImgs:  
				if VERBOSE:
					print "INFO: Processing '%s'" % f 
				tf.write("N")
				img = cv.imread( f ,cv.CV_LOAD_IMAGE_GRAYSCALE)
				for i in xrange (img.shape[1]):
					for j in xrange(img.shape[0]):
						value = img.item(j, i)
						tf.write(",%d"%value)
			tf.write("\n")
	if VERBOSE:
		print "INFO: Done"
		
def prepareTrainingFiles(dsFolder):
	""" """
	global CONFIG
	configFile=join(dsFolder,_DATASET_CONFIG_FILE)
	if not os.path.exist(configFile):
		print "ERR: dataset configuration file '%s' not found" % _DATASET_CONFIG_FILE
		return
	if VERBOSE:
		print "INFO: Reading configuration file at '%s' " %configFile
	execfile(configFile, CONFIG)
	for x in multiclassOnetoOne():
		prepareTrainingFile( x, dsFolder )


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
	prepareTrainingFiles(args.datasetFolder)
