#!/usr/bin/env python
"""
Usage:

    gabor.py img nlambda ntheta sigma ksize size

"""
import numpy as np
import cv2

IMAGES=['training/faces/neutral/S081_008_00000001.png',
        'training/faces/disgust/S005_001_00000011.png',
        'training/faces/anger/S037_003_00000022.png',
        'training/faces/happy/S011_006_00000013.png',
        'training/faces/surprise/S073_001_00000013.png',
        'training/faces/sadness/S042_002_00000016.png',
        'training/faces/contempt/S157_002_00000011.png',
        'training/faces/fear/S011_003_00000014.png'
        ]
SIZE = 24
KSIZE = 7
SIGMA = 2.2
LAMBDA = 10
LAMBDAS = {"beg":8,"end":16}
LAMBDAS_P = [ 
                3, 4, 6, 
                8, 12, 16, 
                24, 36
                ]
THETAS = {"beg":0,"end":np.pi}

def build_filters(nlambda,ntheta):
  filters = []
  ksize = KSIZE
  lambd=LAMBDA
  print(np.arange(LAMBDAS['beg'],LAMBDAS['end'], (LAMBDAS['end']-LAMBDAS['beg'])/float(nlambda)))
  for lambd in np.arange(LAMBDAS['beg'],LAMBDAS['end'], (LAMBDAS['end']-LAMBDAS['beg'])/float(nlambda)):
    for theta in np.arange(THETAS['beg'], THETAS['end'], (THETAS['end']-THETAS['beg']) / float(ntheta)):
      kern = cv2.getGaborKernel((ksize, ksize), SIGMA, 
                theta, lambd, 0.5, 0, ktype=cv2.CV_32F)
      kern /= 1.5*kern.sum()
      filters.append(kern)
  return filters

def process(img2, filters):
  feats=[]
  img=cv2.resize(img2,(SIZE,SIZE))
  accum = np.zeros_like(img)
  void = np.zeros_like(img)
  for kern in filters:
    fimg = cv2.filter2D(img, cv2.CV_8U, kern)
    feats.append(fimg)
    np.maximum(accum, fimg, accum)
  acc_2=accum.astype(np.uint8)
  thr,accum_b=cv2.threshold(acc_2, 170, 255 ,cv2.THRESH_BINARY|cv2.THRESH_OTSU)
  #accum_b=cv2.erode(accum_b, np.ones((2,2),dtype=np.uint8))
  #accum_b=cv2.erode(accum_b, np.ones((3,3),dtype=np.uint8))
  #accum_b=cv2.morphologyEx(accum_b, cv2.MORPH_OPEN, np.ones((2,2),dtype=np.uint8))
  accum_b=cv2.morphologyEx(accum_b, cv2.MORPH_OPEN, np.ones((3,3),dtype=np.uint8))
  return (acc_2,accum_b.astype(np.uint8),feats)

if __name__ == '__main__':
  import sys
  global SIGMA
  global KSIZE
  global SIZE
  global LAMBDAS
  SIGMA = float(sys.argv[3])
  KSIZE= int(sys.argv[4])
  SIZE= int(sys.argv[5])
  LAMBDA=int(sys.argv[1])
  filters = build_filters(int(sys.argv[1]),int(sys.argv[2]))

  res = []
  for img_fn in IMAGES:
    rr=[]
    img = cv2.imread(img_fn, cv2.CV_LOAD_IMAGE_GRAYSCALE)
    res2,res_b,bnk = process(img, filters)
    res1 = cv2.resize(res2,(72,72)) 
    res_b2 = cv2.resize(res_b,(72,72))
    img1 = cv2.resize(img,(72,72)) 
    rr.append(np.rot90(img1))
    rr.append(np.rot90(res1))
    rr.append(np.rot90(res_b2))
    for b in bnk:
      print b.dtype
      bb = cv2.resize(b,(72,72)) 
      rr.append(np.rot90(bb))
    res.append(np.concatenate(rr).transpose())

  final = np.concatenate(res)
  cv2.imshow('result', final)
  key=''
  while key!=ord('q'):
    key=cv2.waitKey(0)
  cv2.destroyAllWindows()
