import cv2
import cv2.cv as cv
#import time


storage=cv.CreateMemStorage()
face_haar=cv.Load('../resources/haarcascade_frontalface_default.xml')
eye_haar=cv.Load('../resources/haarcascade_eye.xml')
#nose_haar=cv.Load('../resources/haarcascade_mcs_nose.xml')
#mouth_haar=cv.Load('workspace/haarcascade_mcs_mouth.xml')
haars=(face_haar,[eye_haar])


def detect1(image , haar_feat, rect=None):
  #t0=time.time()
  rects=list()
  img=cv.fromarray(image)
  containers=cv.HaarDetectObjects(img, haar_feat, storage, 1.1, 2, cv.CV_HAAR_DO_CANNY_PRUNING, rect)
  if containers:
    for f in containers:
      rects.append( (f[0][0],f[0][1],f[0][0]+f[0][2],f[0][1]+f[0][3]) )
    return rects
  else:    
    return None

def detect(image , (haar_container, haar_feats) ):
  #t0=time.time()
  faces=list()
  #img=cv.fromarray(image)
  containers=detect1(image,haar_container,(30,60))
  if containers:
    print containers
    for face in containers:
      # Find other features
      rectlist=list()
      for feat in haar_feats:
        r=detect1(image, feat, (face[1]/5,face[1]/10) ) 
        if r:
          rectlist.extend(r)
      faces.append( (face, rectlist) )
    return faces
    print faces
  else: 
    return None

def draw_rect(image, feat):
  cv.Rectangle(image,(feat[0],feat[1]),(feat[2],feat[3]),cv.RGB(155, 255, 25),2)

def draw_rects(image, faces):
  for face,rectlist in faces:
    draw_rect(image, face)
    for r in rectlist:
      draw_rect(image, r)
  
#image=cv2.imread(sys.argv[1], cv2.CV_LOAD_IMAGE_GRAYSCALE)
#image=cv.fromarray(image)


def main():
  cv2.namedWindow("preview")
  vc = cv2.VideoCapture(0)

  if vc.isOpened():
     rval, frame = vc.read()
  else:
    rval = False

  while rval:    
    feats=detect(frame,haars)
    print feats
    img=cv.fromarray(frame)
    if feats:
      draw_rects(img,feats)
    cv.ShowImage("preview", img) 
    rval, frame = vc.read()
    key = cv2.waitKey(1)
    if key == 27: # exit on ESC
      break

if __name__=='__main__':
  main()


