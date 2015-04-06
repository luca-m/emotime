#! /usr/bin/env python
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
# Copyright © 2015 stk <stk@101337>
#
# Distributed under terms of the MIT license.

"""

"""
import numpy as np
import cv2
import sys

imgbig = cv2.imread(sys.argv[1],0)
img=cv2.resize(imgbig,(200,200))
# create a CLAHE object (Arguments are optional).
clahe = cv2.createCLAHE(clipLimit=3.0, tileGridSize=(8,8))
cl1 = clahe.apply(img)
eq = cv2.equalizeHist(img)
eq1 = cv2.equalizeHist(cl1)


final = np.concatenate((img,eq,cl1,eq1))
cv2.imshow('equalization %s'% sys.argv[1], final)
cv2.waitKey(0)
cv2.destroyAllWindows()

