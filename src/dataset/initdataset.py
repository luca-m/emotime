import os
import argparse

CONFIG={}
DEFAULT_CONFIG_FNAME = "dataset.conf"

def initDatasetDirs(dsPath, dsConfig):
	""" """
	global CONFIG
	execfile(dsConfig, CONFIG)
	for f in CONFIG.CLASSES:
		os.mkdirs(os.path.join(dsPath,f))

	with open( dsConfig,"r") as conf:
		configuration = conf.read()
		with open(os.path.join(dsPath,DEFAULT_CONFIG_FNAME)) as nconf:
			nconf.write(configuration)	

if __name__ == "__main__":
	global VERBOSE
	parser = argparse.ArgumentParser()
	parser.add_argument("dsPath")
	parser.add_argument("config")
	args = parser.parse_args()
	initDatasetDirs(args.dsPath,args.config)