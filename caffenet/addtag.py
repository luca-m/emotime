#! /usr/bin/env python
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
# Copyright © 2016 luca.mella@studio.unibo.it
#
#
# Use it in pipe with:
# find /home/stk/Projects/emotime/dataset/validation/faces/ -type f -exec echo {} \; | python addtag.py > validation.list
# find /home/stk/Projects/emotime/dataset/training/faces/ -type f -exec echo {} \; | python addtag.py > training.list
#

"""

"""
import sys; 
files=sys.stdin.read().split("\n"); 
ft=[]
for f in files: 
  if 'neutral' in f:
    ft.append((f,0))
  elif 'anger' in f:
    ft.append((f,1))
  elif 'contempt' in f:
    ft.append((f,2))
  elif 'disgust' in f:
    ft.append((f,3))
  elif 'fear' in f:
    ft.append((f,4))
  elif 'happy' in f:
    ft.append((f,5))
  elif 'sadness' in f:
    ft.append((f,6))
  elif 'surprise' in f:
    ft.append((f,7))
for fi,tag in ft:
  print ('{} {}'.format(fi,tag))
