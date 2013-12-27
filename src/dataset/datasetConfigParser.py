"""


"""
import ConfigParser as cp
from string import upper

def parse_python_config(configFile):
  config={}
  execfile(configFile, config)
  return config

def parse_ini_config(configFile):
  config={}
  parser=cp.ConfigParser()
  parser.read(configFile)

  classes=[]
  for opt in parser.options('CLASSES'):
    classes.append(opt)
  config['CLASSES']=classes

  for opt in parser.options('FOLDER'):
    config['{0}_FOLDER'.format(upper(opt))]=parser.get('FOLDER',opt)

  for opt in parser.options('SUFFIX'):
    config['{0}_SUFFIX'.format(upper(opt))]=parser.get('SUFFIX',opt)
  
  size={}
  for opt in parser.options('SIZE'):
     size[opt]=parser.get('SIZE',opt)
  config['SIZE']=size
  
  for opt in parser.options('FACECROP'):
    config['FACECROP_{0}'.format(upper(opt))]=parser.get('FACECROP',opt)
  
  for opt in parser.options('GABOR'):
    config['GABOR_{0}'.format(upper(opt))]=parser.get('GABOR',opt)
  
  for opt in parser.options('TRAIN'):
    config['TRAIN_{0}'.format(upper(opt))]=parser.get('TRAIN',opt)

  for opt in parser.options('DETECTION'):
    config['DETECTION_{0}'.format(upper(opt))]=parser.get('DETECTION',opt)

  return config

