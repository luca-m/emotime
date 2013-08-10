#!/usr/bin/env python2

import os
import argparse

from os.path import join

CONFIG={}
DEFAULT_CONFIG_FNAME="dataset.conf"

def initDatasetDirs(dsPath, dsConfig):
	""" """
	global CONFIG
	execfile(dsConfig, CONFIG)
	for clazz in CONFIG['CLASSES']:
		pth = join(dsPath,join(CONFIG['IMAGESFOLDER'],clazz))
		if not os.path.exists(pth):
			os.makedirs(pth)
		pth = join(dsPath,join(CONFIG['FACESFOLDER'],clazz))
		if not os.path.exists(pth):
			os.makedirs(pth)
		pth=join(dsPath,join(CONFIG['FEATURESFOLDER'],clazz))
		if not os.path.exists(pth):
			os.makedirs(pth)
	
	pth=join(dsPath,CONFIG['TRAINFOLDER'])
	if not os.path.exists(pth):
		os.makedirs(pth)
	pth=join(dsPath,CONFIG['CLASSIFIERFOLDER'])
	if not os.path.exists(pth):
		os.makedirs(pth)

	with open( dsConfig,"r") as conf:
		configuration = conf.read()
		with open(join(dsPath,DEFAULT_CONFIG_FNAME),"w") as nconf:
			nconf.write(configuration)	

if __name__ == "__main__":
	global VERBOSE
	parser = argparse.ArgumentParser()
	parser.add_argument("dsPath",help="Dataset folder path")
	parser.add_argument("config", help="Configuration file")
	parser.add_argument("-v","--verbose",action='store_true',help="verbosity")
	args = parser.parse_args()
	if args.verbose:
		VERBOSE=True
	else:
		VERBOSE=False
	initDatasetDirs(args.dsPath,args.config)
