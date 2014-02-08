Emotime
=======

_Recognizing emotional states in faces_

----------------------------------------------
Copyleft [CC-BY-NC 2013](http://creativecommons.org/licenses/by-nc/3.0/)

## Goal
This project aims to recognize main facial expressions (neutral, anger, disgust, fear, joy, sadness, surprise) in image
sequences using the approaches described in:

* [Dynamics of Facial Expression Extracted Automatically from Video](http://ieeexplore.ieee.org/xpl/articleDetails.jsp?arnumber=1384873)
* [Fully Automatic Facial Action Recognition in Spontaneous Behavior](http://ieeexplore.ieee.org/xpl/articleDetails.jsp?arnumber=1613024)

## References

Here is listed some interesting material about machine learning, opencv, gabor transforms and other
stuff that could be useful to get in this topic:

 * [AdaBoost and the Super Bowl of Classifiers](http://www.inf.fu-berlin.de/inst/ag-ki/rojas_home/documents/tutorials/adaboost4.pdf)
 * [Tutorial on Gabor Filters](http://mplab.ucsd.edu/tutorials/gabor.pdf)
 * [Gabor wavelet transform and its application](http://disp.ee.ntu.edu.tw/~pujols/Gabor%20wavelet%20transform%20and%20its%20application.pdf)
 * [Gabor Filter Visualization](http://www.cs.umd.edu/class/spring2005/cmsc838s/assignment-projects/gabor-filter-visualization/report.pdf)
 * [Meta-Analyis of the First Facial Expression Recognition Challenge](http://ieeexplore.ieee.org/xpl/articleDetails.jsp?arnumber=6222016)

## Project Structure

    src
      \-->dataset 		 Scripts for dataset management
      \-->facecrop 		 Utilities and modules for face cropping and registration
      \-->gaborbank		 Utilities and modules for generating gabor filters and image filtering
      \-->adaboost 		 Utilities and modules for adaboost train, prediction, and feature selection
      \-->svm          Utilities and modules for svm training and prediction
      \-->detector     Multiclass detector and preprocessor
      \-->utils        String and IO utilities, CSV supports, and so on..
    doc                Documentation (doxigen)
    resources          Containing third party resources (eg. OpenCV haar classifiers)
    assets             Binary folder
    test               Some testing scripts here

## Build

Dependencies:

* CMake > 2.8
* Python > 2.7, < 3.0
* OpenCV > 2.4.5

Compiling on linux:

* mkdir build; cd build
* cmake .. ; make ; make install

Cross-compiling for windows:

* TODO: CMake Toolchain for MinGW

Compiling on linux:

* TODO: CMakeGui

## Training

### Dataset

The [Cohn-Kanade database](http://www.consortium.ri.cmu.edu/ckagree/) is one of the most used faces database. Its extended version (CK+) contains also [FACS](http://en.wikipedia.org/wiki/Facial_Action_Coding_System)
code labels (aka Action Units) and emotion labels (neutral, anger, contempt, disgust, fear, happy, sadness, surprise).

### Usage

Initialize and fill a dataset:

    python2 datasetInit.py [-h] dsPath config
    python2 datasetFillCK.py [-h] datasetFolder cohnKanadeFolder cohnKanadeEmotionsFolder

Train some models:

    ./train_models.sh svm 1vsallext ./dataset

Or also:

    python datasetCropFaces.py [-h] [--eye-correction] dsFolder
    python datasetFeatures.py [-h] dsFolder 
    python datasetPrepTrain.py [-h] [--mode {1vs1,1vsAll,1vsAllExt}] dsFolder
    python datasetTrain.py [-h] [--mode {adaboost,svm}] dsFolder 
    python datasetVerifyPrediction.py [-h] [--mode {adaboost,svm}] [--eye-correction] dsFolder

Training with _native_ tool:

    TBD

## Detection and Prediction

### Usage


Video gui:

    echo "<VIDEOPATH>"|./emotimevideo_cli FACEDETECTORXML (EXEDETECTORXML|none) WIDTH HEIGHT NWIDTHS NLAMBDAS NTHETAS (svm|ada) (TRAINEDCLASSIFIERSXML)+

Cam gui:

    ./emotimegui_cli FACEDETECTORXML (EXEDETECTORXML|none) WIDTH HEIGHT NWIDTHS NLAMBDAS NTHETAS (svm|ada) (TRAINEDCLASSIFIERSXML)+

Or:

    ./gui.py --mode svm --eye-correction <dataset_path>

## Further Development

* Validation dataset
* Much more training samples, some emotions in Cohn-Kanade are under-sampled.
* Better GUI
* Better CLI tools

