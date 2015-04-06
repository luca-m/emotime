#! /usr/bin/env python
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
# Copyright © 2015 stk <stk@101337>
#
# Distributed under terms of the MIT license.
import cv2
import cv2
import numpy
import sys 

imgFile= numpy.asarray(cv2.cv.Load(sys.argv[1]))
img8=imgFile.astype(numpy.uint8)
#cv2.equalizeHist(img8,img8)
cv2.imshow('gabor %s'% sys.argv[1], img8)
while cv2.waitKey(0) != ord('q'):
    pass
cv2.destroyAllWindows()
