"""


"""
import ConfigParser as cp
from string import upper

def fill_it(parser, name, left_side):

  if left_side:
    fstr = '{0}_{1}'
  else:
    fstr = '{1}_{0}'

  config = {}
  for opt in parser.options(name):
    val = parser.get(name,opt)
    if upper(opt) == 'TOOL':
      try:
        with open(val):
          pass
      except:
        val += '.exe'
        with open(val):
          pass
    config[fstr.format(upper(opt), name)] = val

  return config

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

  size={}
  for opt in parser.options('SIZE'):
     size[opt]=parser.get('SIZE',opt)
  config['SIZE']=size


  config.update(fill_it(parser, 'FOLDER', True))
  config.update(fill_it(parser, 'SUFFIX', True))

  config.update(fill_it(parser, 'FACECROP', False))
  config.update(fill_it(parser, 'GABOR', False))
  config.update(fill_it(parser, 'TRAIN', False))
  config.update(fill_it(parser, 'DETECTION', False))
  config.update(fill_it(parser, 'GUI', False))

  return config

