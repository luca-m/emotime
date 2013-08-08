#!/usr/bin/env python2

import cv2 as cv
import os
import itertools
import argparse

# TODO: path from parameters 

_DATASET_CONFIG_FILE = ".."+ os.path.sep + "dataset" + os.path.sep + "dataset.conf"
_TRAINING_FILE_PATH = ".."+ os.path.sep + "dataset" + os.path.sep + "training" + os.path.sep

VERBOSE=False
CONFIG={}


def multiclassOnetoOne():
	""" """
	return [x for x in itertools.permutations( CONFIG.CLASSES ,2 )]

def prepareTrainingFile( (goodClass,badClass),  dsFolder, outFolder):
	""" """
	if VERBOSE:
		print "INFO: Preparing training file for '%s' vs. '%s' " %(goodClass,badClass)
	
	goodPath=dsFolder+os.path.sep+goodClass
	badPath=dsFolder+os.path.sep+badClass
	#
	# Note: a goodFolder should contain the filtered images (various orientation and frequency ) 
	#       of a single sample image. So each line of the training file will be composed by its
	#		MARKER ( Good or Bad ) plus all the pixel values of the filtered images 
	#
	goodFolders=[ f for f in os.listdir(goodPath) if os.path.isdir(os.path.join(goodPath,f)) and f.endswith(CONFIG.FILTERED_FOLDER_SUFFIX) ]
	badFolders=[ f for f in os.listdir(badPath) if os.path.isdir(os.path.join(badPath,f)) and f.endswith(CONFIG.FILTERED_FOLDER_SUFFIX) ]

	outfpath=os.path.join(outFolder,goodClass+"_"+badClass+CONFIG.FEATURE_FILE_SUFFIX)
	with open( outfpath , "w") as tf:
		# Open gabor filtered images for each sample and prepare csv row
		for fold in goodFolders:
			goodImgs=[ f for f in os.listdir(fold) if os.path.isfile(os.path.join(fold,f))]
			goodImgs.sort()
			for f in goodImgs:  
				tf.write("G")
				img = cv.imread( f ,cv.CV_LOAD_IMAGE_GRAYSCALE)
				for i in xrange (img.shape[1]):
					for j in xrange(img.shape[0]):
						value = img.item(j, i)
						tf.write(",%d"%value)
			tf.write("\n")
		for fold in badFolders:
			badImgs=[f for f in os.listdir(fold) if os.path.isfile(os.path.join(fold,f))]
			badImgs.sort()
			for f in badImgs:  
				tf.write("B")
				img = cv.imread( f ,cv.CV_LOAD_IMAGE_GRAYSCALE)
				for i in xrange (img.shape[1]):
					for j in xrange(img.shape[0]):
						value = img.item(j, i)
						tf.write(",%d"%value)
			tf.write("\n")
	if VERBOSE:
		print "INFO: Done"
		
def prepareTrainingFiles(configFile, dsFolder):
	""" """
	global CONFIG
	if VERBOSE:
		print "INFO: Reading configuration file at '%s' " %configFile
	execfile(configFile, CONFIG)
	outFolder=op.path.join(dsFolder,CONFIG.TRAINFOLDER)
	for x in multiclassOnetoOne():
		prepareTrainingFile( x, dsFolder, outFolder )



if __name__ == "__main__":
	global VERBOSE
	parser = argparse.ArgumentParser()
	parser.add_argument("datasetCfg", help="Dataset config file path")
	parser.add_argument("datasetFolder",help="Dataset folder")
	parser.add_argument("--verbose",help="increase output verbosity")
	args = parser.parse_args()
	if ()
	prepareTrainingFiles(args.datasetCfg,args.datasetFolder)