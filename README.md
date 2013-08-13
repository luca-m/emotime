Emotime
=======

Recognizing emotional states in faces

## Goal
This project aim to recognize main facial expressions (neutral, anger, disgust, fear, joy, sadness, surprise) in image 
sequence using the approaches described in:

* [Dynamics of Facial Expression Extracted Automatically from Video](http://ieeexplore.ieee.org/xpl/articleDetails.jsp?arnumber=1384873)
* [Fully Automatic Facial Action Recognition in Spontaneous Behavior](http://ieeexplore.ieee.org/xpl/articleDetails.jsp?arnumber=1613024)

## References

Here is listed some interesting material about machine learning, opencv, gabor transforms and other 
stuff that could be usefull to get in this topic:

 * [AdaBoost and the Super Bowl of Classifiers](http://www.inf.fu-berlin.de/inst/ag-ki/rojas_home/documents/tutorials/adaboost4.pdf)
 * [Tutorial on Gabor Filters](http://mplab.ucsd.edu/tutorials/gabor.pdf)
 * [Gabor wavelet transform and its application](http://disp.ee.ntu.edu.tw/~pujols/Gabor%20wavelet%20transform%20and%20its%20application.pdf)
 * [Gabor Filter Visualization](http://www.cs.umd.edu/class/spring2005/cmsc838s/assignment-projects/gabor-filter-visualization/report.pdf)

## Project Structure

    build                     # where to build
    assets                    # binary folder (will contains building outcome)
    resources                 # contains some useful resources like opencv haartraining files
    dataset                   # a folder where to initialize a dataset
    src
      \-- dataset             # utilities for handling dataset
      \-- gaborbank           # utility for calculating gabor filters
      \-- facecrop            # utility for cropping and registering faces
      \-- adaboost            # utilities for training and use adaBoost classifier

## Building

    cd build
    cmake .. ; make ; make install

Note: make install actually doesn't install anything in the system, it will copy all binaries in the asset folder.

## Usage

##### Dataset initialization

    ./initdataset.py [-h] [-v] dsPath config

##### Fill dataset with CK+ database

    ./filldatasetCohnKanade.py [-h] [-v] datasetFolder cohnKanadeFolder cohnKanadeEmotionsFolder
    
##### Crop all faces from imported images

     ./cropFaces.py [-h] [-v] datasetFolder faceDetectorCfg
     
##### Calculate gabor filtered images

    ./calcFeatures.py [-h] [-v] datasetFolder
    
##### Prepare training files (CSV format)

    ./trainfiles.py [-h] [-v] datasetFolder


## Dataset

The [Cohn-Kanade database](http://www.consortium.ri.cmu.edu/ckagree/) is one of the most used face database. In it's extended version (CK+) it contains also [FACS](http://en.wikipedia.org/wiki/Facial_Action_Coding_System) 
code labels (aka Action Units) and emotion labels (neutral, anger, contempt, disgust, fear, happy, sadness, surprise).


----------------------------------------------
Copyleft [CC-BY-NC 2013](http://creativecommons.org/licenses/by-nc/3.0/)

