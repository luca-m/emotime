#! /usr/bin/env python
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
# Distributed under terms of the MIT license.

"""

"""
import cv
import cv2
import sys
import math
from math import sqrt
import numpy as np

def search_eyes(image,eye_cascade):
  detectedEyes = eye_cascade.detectMultiScale(image,1.3,5)
  eyepos=[]
  if detectedEyes is not None:
    eye1=(0,0)
    eye2=(0,0)
    val1=-1
    val2=-1
    for (x,y,w,h) in detectedEyes:
      area=w*h
      if area>val1 and val1>val2:
       tmp=val1
       tmpe=eye1
       val1=area
       eye1=(x+w/2, y+h/2)
       val2=tmp
       eye2=tmpe
      elif area>val2 and val2>val1:
       tmp=val2
       tmpe=eye2
       val2=area
       eye2=(x+w/2, y+h/2)
       val1=tmp
       eye1=tmpe
      elif area>val1 : 
        # second
        val1=area
        eye1=(x+w/2, y+h/2)
      elif area>val2 :
        # second
        val2=area
        eye2=(x+w/2, y+h/2)
    eyepos.append(eye1)
    eyepos.append(eye2)
    print "eyepos=%s"%str(eyepos)
    left=(0,0)
    right=(0,0)
    upper=(0,0)
    lower=(0,0)
    if len(eyepos)>1:
       if eye1[0]<eye2[0]:
         left=eye1
         right=eye2
       else:
         right=eye1
         left=eye2
       if eye1[1]<eye2[1]:
         lower=eye1
         upper=eye2
       else:
         upper=eye1
         lower=eye2
    trianglebase=(upper[0],lower[1])
    eyecenter=(left[0]+(right[0]-left[0])/2, lower[1]+(upper[1]-lower[1])/2)
    print "eyecenter=%s"%str(eyecenter)
    print "trianglebase=%s"%str(trianglebase)
    cv2.rectangle(image,(eyecenter[0], eyecenter[1]),
                   (eyecenter[0]+1,eyecenter[1]+1),
                   cv.RGB(155, 255, 200),2)
    cv2.rectangle(image,(trianglebase[0], trianglebase[1]),
                   (trianglebase[0]+1,trianglebase[1]+1),
                   cv.RGB(255, 0, 200),2)
    c0=math.sqrt((trianglebase[0]-upper[0])**2+(trianglebase[1]-upper[1])**2)
    c1=math.sqrt((trianglebase[0]-lower[0])**2+(trianglebase[1]-lower[1])**2)
    ip=math.sqrt((upper[0]-lower[0])**2+(upper[1]-lower[1])**2)
    print c0,c1,ip,c1/ip
    angle=math.degrees(math.acos(c1/ip))
    print angle
    rotMat=cv2.getRotationMatrix2D((eyecenter), angle, 1.0)
    return cv2.warpAffine(image, rotMat, img.shape, flags=cv2.INTER_LINEAR) 

img=cv2.imread(sys.argv[1], cv2.CV_LOAD_IMAGE_GRAYSCALE)
eye_cascade = cv2.CascadeClassifier('haarcascade_eye.xml')
im=search_eyes(img,eye_cascade)
#im=search_eyes(im,eye_cascade)
cv2.imshow('original',img)
cv2.imshow('registered',im)
cv.WaitKey()
