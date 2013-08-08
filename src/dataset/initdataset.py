import os
import argparse

from os.path import join

CONFIG={}
DEFAULT_CONFIG_FNAME = "dataset.conf"

def initDatasetDirs(dsPath, dsConfig):
	""" """
	global CONFIG
	execfile(dsConfig, CONFIG)
	for clazz in CONFIG['CLASSES']:
		os.mkdirs(join(dsPath,join(CONFIG['IMAGESFOLDER'],clazz)))
		os.mkdirs(join(dsPath,join(CONFIG['FACESFOLDER'],clazz)))
		os.mkdirs(join(dsPath,join(CONFIG['FEATURESFOLDER'],clazz)))
	
	os.mkdirs(join(dsPath,CONFIG['TRAINFOLDER']))		
	os.mkdirs(join(dsPath,CONFIG['CLASSIFIERFOLDER']))		

	with open( dsConfig,"r") as conf:
		configuration = conf.read()
		with open(join(dsPath,DEFAULT_CONFIG_FNAME)) as nconf:
			nconf.write(configuration)	

if __name__ == "__main__":
	global VERBOSE
	parser = argparse.ArgumentParser()
	parser.add_argument("dsPath")
	parser.add_argument("config")
	args = parser.parse_args()
	initDatasetDirs(args.dsPath,args.config)
