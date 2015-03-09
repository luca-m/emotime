Emotime
=======

_Recognizing emotional states in faces_

----------------------------------------------

Authors: Luca Mella, Daniele Bellavista

Development Status: Experimental 

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


_NOTE: watch for illumination! At the moment optimal results can be obtained in live webcam sessions using direct illumination directed to the user's face. Don't worry you are not required to blind you with a headlight._

__If you'd like to try emotime without any further complication you should take a look to the [x86_64 release](https://github.com/luca-m/emotime/releases/tag/v1.1-experimental).__


### Usage

Video gui:

    echo "VIDEOPATH" | ./emotimevideo_cli FACEDETECTORXML (EYEDETECTORXML|none) WIDTH HEIGHT NWIDTHS NLAMBDAS NTHETAS (svm|ada) (TRAINEDCLASSIFIERSXML)+

Cam gui:

    ./emotimegui_cli FACEDETECTORXML (EYEDETECTORXML|none) WIDTH HEIGHT NWIDTHS NLAMBDAS NTHETAS (svm|ada) (TRAINEDCLASSIFIERSXML)+

Or using the python script:

    python gui.py --cfg <dataset_configuration_path> --mode svm --eye-correction <dataset_path>

#### Binary Release and Trained Models

If you just want to take a quick look to the project we strongly suggest to go to the [release section](https://github.com/luca-m/emotime/releases) and download compiled binaries for _Linux 64bit_, then:

* download and unzip the binaries in an empty folder
* run `./download_trained_models.sh`
* Then cd assets and `./emotimegui_cli ../resources/haarcascade_frontalface_cbcl1.xml none 48 48 3 5 4 svm ../dataset_svm_354_cbcl1_1vsallext/classifiers/svm/*`


## Training

After `mkdir build; cd build; cmake ..; make ; make install` go to the `assets` folder and:

1. Initialize a dataset using:

        python datasetInit.py -cfg <CONFIGFILE> <EMPTY_DATASET_FOLDER>

2. Then fill it with your images or use the Cohn-Kanade importing script:

        python datasetFillCK --cfg <CONFIGFILE> <DATASETFOLDER> <CKFOLDER> <CKEMOTIONFOLDER>

3. Now you are ready to train models:

        python train_models.py --cfg <CONFIGFILE> --mode svm --prep-train-mode [1vsall|1vsallext] <DATASETFOLDER>


### Dataset

The [Cohn-Kanade database](http://www.consortium.ri.cmu.edu/ckagree/) is one of the most used faces database. Its extended version (CK+) contains also [FACS](http://en.wikipedia.org/wiki/Facial_Action_Coding_System)
code labels (aka Action Units) and emotion labels (neutral, anger, contempt, disgust, fear, happy, sadness, surprise).

## Validation

_First, rough evaluation of the performance of the system_ 
Validation test involved the whole system `face detector + emotion classifier`, so should not be considered relative to the _emotion classifier_ itself. 

Of course, a more fine validation shuld be tackled in order to evaluate emotion classifier alone.
For the sake of completeness the reader have to know that the _cbcl1_ face model is a good face locator rather than detector, roughly speaking it detects less but is more precise.

Following results are commented with my personal - totally informal - evaluation after live webcam session.

```text
multicalss method: 1vsAllExt 
face detector:     cbcl1
eye correction:    no 
width:             48
height:            48 
nwidths:           3 
nlambdas:          5
nthetas:           4

Sadness                   <-- Not good in live webcam sessions too
  sadness -> 0.67%
  surprise -> 0.17%
  anger -> 0.17%
Neutral                   <-- Good in live webcam sessions
  neutral -> 0.90%
  contempt -> 0.03%
  anger -> 0.03%
  fear -> 0.02%
  surprise -> 0.01%
Disgust                   <-- Good in live webcam sessions
  disgust -> 1.00%
Anger                     <-- Good in live webcam sessions
  anger -> 0.45%
  neutral -> 0.36%
  disgust -> 0.09%
  contempt -> 0.09%
Surprise                  <-- Good in live webcam sessions
  surprise -> 0.94%
  neutral -> 0.06%
Fear                      <-- Almost Good in live webcam sessions
  fear -> 0.67%
  surprise -> 0.17%
  happy -> 0.17%
Contempt                  <-- Not good in live webcam sessions
  neutral -> 0.50%
  contempt -> 0.25%
  anger -> 0.25%
Happy                     <-- Good in live webcam sessions
  happy -> 1.00%
```

```text
multicalss method: 1vsAll 
face detector:     cbcl1
eye correction:    no 
width:             48
height:            48 
nwidths:           3 
nlambdas:          5
nthetas:           4

Sadness                   <-- Not good in live webcam sessions too
  unknown -> 0.50%
  sadness -> 0.33%
  fear -> 0.17%
Neutral                   <-- Good in live webcam sessions 
  neutral -> 0.73%
  unknown -> 0.24%
  surprise -> 0.01%
  fear -> 0.01%
  contempt -> 0.01%
Disgust                   <-- Good in live webcam sessions
  disgust -> 0.82%
  unknown -> 0.18%
Anger                     <-- Almost sufficient in live webcam sessions
  anger -> 0.36%
  neutral -> 0.27%
  unknown -> 0.18%
  disgust -> 0.09%
  contempt -> 0.09%
Surprise                  <-- Good in live webcam sessions
  surprise -> 0.94%
  neutral -> 0.06%
Fear                      <-- Sufficient in live webcam sessions
  fear -> 0.67%
  surprise -> 0.17%
  happy -> 0.17%
Contempt                  <-- Not good in live webcam sessions too
  unknown -> 1.00%
Happy                     <-- Good in live webcam sessions 
  happy -> 1.00%
```

Also main difference between the _1vsAll_ and the _1vsAllExt_ mode experimented in livecam sessions are related to the amount of unknown states registered and the stability of the detected states.
In detail 1vsAll multiclass method provide more less noisy detections during a live web-cam session, 1vsAllExt mode instead is able to always predict a valid state for each frame processed, but sometimes it result to be more unstable during the expression transition.


Sorry for the lack of fine tuning and detail, but it is a spare time project at the moment..
If you have any idea or suggestion feel free to write us!


## Further Development

* Tuning GaborBank parameters for accuracy enhancement.
* Tuning image sizes for better real-time performance.
* Better handle illumination, detections are good when frontal light is in place (keep it in mind when you use it with your camera).
