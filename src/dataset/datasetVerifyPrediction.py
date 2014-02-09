#!/usr/bin/env python2

import argparse
import sys
import datasetConfigParser as dcp
import os
import subprocess
import re

from subprocess import PIPE

def dataset_verify_prediction(dsfolder, config, mode, eye_detection):
  results = dataset_do_prediction(dsfolder, config, mode, eye_detection)
  dataset_process_results(results)

def dataset_process_results(results):
  for expected, l in results.items():
    percs = {}
    for (res, val) in l:
      if not percs.has_key(res):
        percs[res] = 1.0 / len(l)
      else:
        percs[res] += 1.0 / len(l)
    percs = sorted([(v, r) for r,v in percs.items()], reverse=True)

    print expected.capitalize()
    for (v, k) in percs:
      print "\t", k, '->', "%.2f%%"%v


def dataset_do_prediction(dsfolder, config, mode, eye_detection, do_prints=True):
  faces_dir = os.path.join(dsfolder, config['IMAGES_FOLDER'])

  if mode == 'svm':
    class_dir = os.path.join(dsfolder, config['CLASSIFIER_SVM_FOLDER'])
    mode_s = 'svm'
  else:
    class_dir = os.path.join(dsfolder, config['CLASSIFIER_FOLDER'])
    mode_s = 'ada'

  execut = config['DETECTION_TOOL']
  print "INFO: detector tool '%s %s', eye detection: %r"%(execut, mode_s, eye_detection)

  classificators = []
  for f in os.listdir(class_dir):
    abs_f = os.path.join(class_dir, f)
    if os.path.isfile(abs_f):
      classificators.append(abs_f)

  #print "INFO: classifiers %s"%str(classificators)

  results = {}
  args = [execut, mode_s, config['FACECROP_FACE_DETECTOR_CFG']]
  if eye_detection:
    args.append(config['FACECROP_EYE_DETECTOR_CFG'])
  else:
    args.append('none')
  args += [config['SIZE']['width'], config['SIZE']['height'],
      config['GABOR_NWIDTHS'], config['GABOR_NLAMBDAS'],
      config['GABOR_NTHETAS']] + classificators

  res_reg =  re.compile("predicted: (\w*) with score (.*)")
  for emo in os.listdir(faces_dir):
    emo_dir = os.path.join(faces_dir, emo)

    if not os.path.isdir(emo_dir):
      continue

    faces_list = [os.path.join(emo_dir, f) for f in os.listdir(emo_dir)]
    faces = '\n'.join(faces_list)
    if do_prints:
      print "Predicting:", emo, "(%d faces)"%len(faces_list)

    p = subprocess.Popen(args, stdout=PIPE, stdin=PIPE, stderr=PIPE)
    out = p.communicate(input=faces)
    results[emo] = re.findall(res_reg, out[0])

  if do_prints:
    print ""
  return results

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
    config = dcp.parse_ini_config(args.cfg)
    dataset_verify_prediction(args.dsFolder, config, args.mode, args.eye_correction)
  except Exception as e:
    print "ERR: something wrong (%s)" % str(e)
    sys.exit(1)
