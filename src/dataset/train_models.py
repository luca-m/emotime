#!/usr/bin/python2

import argparse
import subprocess
import sys
import os

if __name__ == "__main__":

  parser = argparse.ArgumentParser()
  parser.add_argument("--cfg", help="Dataset config file name", default='dataset.cfg')
  parser.add_argument("dsFolder", help="Dataset base folder")
  parser.add_argument("--mode", choices=['adaboost', 'svm'], help="training mode: adaboost or svm", required=True)
  parser.add_argument("--prep-train-mode", choices=['1vsallext', '1vsall', '1vs1'], help="Training set preparation mode: 1vsall, 1vsall extended, 1vs1", required=True)
  parser.add_argument("--eye-correction", action="store_true", help="Apply eye correction to faces")
  parser.add_argument("--skip-facecrop", action="store_true", help="WARNING: To be set only if the facecropping was already performed with the same configuration file!")
  parser.add_argument("--skip-feature", action="store_true", help="WARNING: To be set only if the (gabor) feature calculation was already performed with the same configuration file!")
  parser.add_argument("--clean", action="store_true", help="Clean training dataset before start")

  args = parser.parse_args()

  print(" ** Emotime ** ")
  print("   > Luca Mella")
  print("   > Daniele Bellavista")
  print("")
  print(" [*] Remember! Before using the training, the dataset folder must be")
  print("     initialized with datasetInit.py and datasetFillCK.py")
  print("")

  if not os.path.isfile(args.cfg):
    print(" [#] ERROR: configuration file", args.cfg, "doesn't exists at path", os.path.abspath(args.cfg))
    print("")
    sys.exit(1)

  if not os.path.isdir(args.dsFolder):
    print(" [#] ERROR: dataset folder", args.dsFolder, "doesn't exists at path", os.path.abspath(args.dsFolder))
    print("")
    sys.exit(1)

  print(" [*] Parameters: ")
  print("   [>] Dataset folder: " + args.dsFolder)
  print("   [>] Configuration file: " + args.cfg)
  print("   [>] Trainig mode: " + args.mode)
  print("   [>] Trainig preparation mode: " + args.prep_train_mode)
  print("   [>] Eye correction: " + repr(args.eye_correction))
  print("   [>] Skip facecrop (WARNING): " + repr(args.skip_facecrop))

  base_args = ['--cfg', args.cfg, args.dsFolder]
  prep_mode_args = ['--mode', args.prep_train_mode]
  mode_args = ['--mode', args.mode]
  eye_args = []
  if args.eye_correction:
    eye_args.append('--eye-correction')


########################### Face Cropping
  if not args.skip_facecrop:
    print(" [1] Cropping faces...")

    if subprocess.call(['python', './datasetCropFaces.py'] + eye_args + base_args) is not 0:
      print(" [#] An Error occured! Exiting...")
      sys.exit(1)
  else:
    print(" [1] Skipping face crop!")
##########################################

########################### Features computation
  if not args.skip_feature:
    print(" [2] Computing features using bank of gabor magniture filters...")

    if subprocess.call(['python', './datasetFeatures.py'] + base_args) is not 0:
      print(" [#] An Error occured! Exiting...")
      sys.exit(1)
  else:
    print(" [2] Skipping feature calculation!")
##########################################

########################### Training files preparation
  print(" [3] Preparing CSV files with training data using %s..."%args.prep_train_mode)

  # Removing old csv files
  train_fold = os.path.join(args.dsFolder, 'training')
  [os.remove(os.path.join(train_fold, f)) for f in os.listdir(train_fold) if f.endswith(".csv")]

  if subprocess.call(['python', './datasetPrepTrain.py'] + prep_mode_args + base_args) is not 0:
    print(" [#] An Error occured! Exiting...")
    sys.exit(1)
##########################################

########################### Training
  print(" [4] Training with %s and selecting relevant features..."%args.mode)

  if subprocess.call(['python', './datasetTrain.py'] + mode_args + base_args) is not 0:
    print(" [#] An Error occured! Exiting...")
    sys.exit(1)
##########################################

########################### Verification
  print(" [5] Verifying the prediction...")

  if subprocess.call(['python', './datasetVerifyPrediction.py'] + mode_args + eye_args + base_args) is not 0:
    print(" [#] An Error occured! Exiting...")
    sys.exit(1)
##########################################
