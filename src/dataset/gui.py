#!/usr/bin/env python2

import argparse
import datasetConfigParser as dcp
import os


def launch_gui(dsfolder, config, mode, eye_detection, do_prints=True):

  if mode == 'svm':
    class_dir = os.path.join(dsfolder, config['CLASSIFIER_SVM_FOLDER'])
    mode_s = 'svm'
  else:
    class_dir = os.path.join(dsfolder, config['CLASSIFIER_FOLDER'])
    mode_s = 'ada'

  execut = config['GUI_TOOL']

  classificators = []
  for f in os.listdir(class_dir):
    abs_f = os.path.join(class_dir, f)
    if os.path.isfile(abs_f):
      classificators.append(abs_f)

  args = [execut, config['FACECROP_FACE_DETECTOR_CFG']]
  if eye_detection:
    args.append(config['FACECROP_EYE_DETECTOR_CFG'])
  else:
    args.append('none')
  args += [config['SIZE']['width'], config['SIZE']['height'],
      config['GABOR_NWIDTHS'], config['GABOR_NLAMBDAS'],
      config['GABOR_NTHETAS'], mode_s] + classificators

  print "Launching emotime gui! Smile :)"
  os.execv(execut, args);


if __name__ == "__main__":
  parser = argparse.ArgumentParser()
  parser.add_argument("--cfg", default="dataset.cfg", help="Dataset config file name")
  parser.add_argument("dsFolder", help="Dataset base folder")
  parser.add_argument("-v", "--verbose", action='store_true', help="verbosity")
  parser.add_argument("--mode", default="adaboost", choices=['adaboost', 'svm'], help="training mode: adaboost or svm")
  parser.add_argument("--eye-correction", action="store_true", help="Perform eye correction on images")
  args = parser.parse_args()

  try:
    config = {}
    config = dcp.parse_ini_config(os.path.join(args.dsFolder, args.cfg))
    launch_gui(args.dsFolder, config, args.mode, args.eye_correction)
  except Exception as e:
    print "ERR: something wrong (%s)" % str(e)
