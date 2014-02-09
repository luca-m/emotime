Emotime
=======

_Recognizing emotional states in faces_

----------------------------------------------

Authors: Luca Mella, Daniele Bellavista

Development Status: Prototype 

Copyleft: [CC-BY-NC 2013](http://creativecommons.org/licenses/by-nc/3.0/)

----------------------------------------------

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
    report             Class project report (latex)
    resources          Containing third party resources (eg. OpenCV haar classifiers)
    assets             Binary folder
    test               Some testing scripts here

## Build

Dependencies:

* `CMake >= 2.8`
* `Python >= 2.7, < 3.0`
* `OpenCV >= 2.4.5`

Compiling on linux:

* `mkdir build` 
* `cd build`
* `cmake .. ; make ; make install` - now the `asset` folder should be populated

Cross-compiling for windows:

* Using CMake or CMakeGUI, select emotime as source folder and configure.
* If it complains about setting the variable `OpenCV_DIR` set it to the appropriate path so that:
  - C:/path/to/opencv/dir/ contains the libraries (`*.lib`)
  - C:/path/to/opencv/dir/include contains the include directories (opencv and opencv2)
  - **IF the include directory is missing** the project will likely not be able
    to compile due to missing reference to `opencv2/opencv` or similar.
* Then generate the project and compile it.
* This was tested with Visual Studio 12 64 bit.

## Detection and Prediction

Proof of concept model trained using faces extracted using the detector `cbcl1` are available for download, mulclass strategy [1 vs all](https://dl.dropboxusercontent.com/u/7618747/dataset_svm_354_cbcl1_1vsall.zip) and [many vs many](https://dl.dropboxusercontent.com/u/7618747/dataset_svm_354_cbcl1_1vsallext.zip) can be found.

_NOTE: remember that this is a prototype_

### Usage

Video gui:

    echo "VIDEOPATH" | ./emotimevideo_cli FACEDETECTORXML (EYEDETECTORXML|none) WIDTH HEIGHT NWIDTHS NLAMBDAS NTHETAS (svm|ada) (TRAINEDCLASSIFIERSXML)+

Cam gui:

    ./emotimegui_cli FACEDETECTORXML (EYEDETECTORXML|none) WIDTH HEIGHT NWIDTHS NLAMBDAS NTHETAS (svm|ada) (TRAINEDCLASSIFIERSXML)+

Or:

    ./gui.py --mode svm --eye-correction <dataset_path>


## Training

### Dataset

The [Cohn-Kanade database](http://www.consortium.ri.cmu.edu/ckagree/) is one of the most used faces database. Its extended version (CK+) contains also [FACS](http://en.wikipedia.org/wiki/Facial_Action_Coding_System)
code labels (aka Action Units) and emotion labels (neutral, anger, contempt, disgust, fear, happy, sadness, surprise).

### Usage

Initialize and fill a dataset:

    python2 datasetInit.py [-h] --cfg dataset.cfg dsPath
    python2 datasetFillCK.py [-h] datasetFolder cohnKanadeFolder cohnKanadeEmotionsFolder

Train some models:

    ./train_models.sh svm 1vsallext ./dataset

Or better:

    ./train_models.py --cfg dataset.cfg --mode svm --prep-train-mode 1vsAll --eye-correction dsFolder

Or also:

    python datasetCropFaces.py [-h] [--eye-correction] dsFolder
    python datasetFeatures.py [-h] dsFolder 
    python datasetPrepTrain.py [-h] [--mode {1vs1,1vsAll,1vsAllExt}] dsFolder
    python datasetTrain.py [-h] [--mode {adaboost,svm}] dsFolder 
    python datasetVerifyPrediction.py [-h] [--mode {adaboost,svm}] [--eye-correction] dsFolder

Training with _native_ tool:

    TBD

## Further Development

* Validation dataset
* Finest parameter tuning
* Extend training dataset

