Emotime
=======

_Recognizing emotional states in faces_

----------------------------------------------
Copyleft [CC-BY-NC 2013](http://creativecommons.org/licenses/by-nc/3.0/)

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
 * [Meta-Analyis of the First Facial Expression Recognition Challenge](http://ieeexplore.ieee.org/xpl/articleDetails.jsp?arnumber=6222016)

## Project Structure

```
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
```

## Build

Dependencies:

* CMake > 2.8
* Python > 2.7, < 3.0 
* OpenCV > 2.4.5

~~_NOTE: OpenCV CvMLData is used for loading training files, but if you try to run the ML-Training procedure as is you could encounter some problem related to OpenCV default values and size of training data. For the sake of simplicity you MAY NEEED to make CvMLData.read.csv capable of reading bigger data. In order to do it you should RECOMPILE and REINSTALL OpenCV after a very quick modification. In detail you should go to `opencv-2.x.x` folder, then edit `modules/ml/src/data.cpp`, find the `read_csv` method, find the `storage = cvCreateMemStorage();` line and specify a bigger value than the default one (eg.`storage = cvCreateMemStorage(256*2048)` instead of 64K)_~~

Compiling on linux:

* mkdir build; cd build
* cmake .. ; make ; make install

Cross-compiling for windows:

* TODO: CMake Toolchain for MinGW

Compiling on linux:

* TODO: CMakeGui

## Training

### Dataset

The [Cohn-Kanade database](http://www.consortium.ri.cmu.edu/ckagree/) is one of the most used face database. In it's extended version (CK+) it contains also [FACS](http://en.wikipedia.org/wiki/Facial_Action_Coding_System) 
code labels (aka Action Units) and emotion labels (neutral, anger, contempt, disgust, fear, happy, sadness, surprise).

### Usage

Initialize and fill a dataset:

    python2 datasetInit.py [-h] dsPath config
    python2 datasetFillCK.py [-h] datasetFolder cohnKanadeFolder cohnKanadeEmotionsFolder

Train some models:

    ./train_models.sh svm 1vsallext ./dataset

Or also :

    python datasetCropFaces.py [-h] [--eye-correction] dsFolder
    python datasetFeatures.py [-h] dsFolder 
    python datasetPrepTrain.py [-h] [--mode {1vs1,1vsAll,1vsAllExt}] dsFolder
    python datasetTrain.py [-h] [--mode {adaboost,svm}] dsFolder 
    python datasetVerifyPrediction.py [-h] [--mode {adaboost,svm}] [--eye-correction] dsFolder

Training with _native_ tool:

    TBD

### Training Results

#### Boosting

_WARNING: AdaBoost models training and features selection has been SUSPENDED due to the HUGE amount of training time. Man we have laptops.._

Training parameters:

* Face detector: `haarcascade_frontalface_alt.xml`, `no rotation correction`
* Size: `32x32`
* Gabor kernels: `nwidths=1`, `nlambdas=5`, `nthetas=8`
* Multiclass mode: 1vsAll

AdaBoost opencv _realboost_ results (no neutral, only true positive):

``` 
anger : 39/45   -  .8666666666
contempt : 8/18   -  .4444444444
disgust : 52/59   -  .8813559322
fear : 18/25   -  .7200000000
happy : 65/69   -  .9420289855
neutral : 0/0   -  
sadness : 23/28   -  .8214285714
surprise : 81/83   -  .9759036144
Train hit rate: 286/327   -  .8746177370
```
AdaBoost opencv _gentleboost_ results (no neutral, only true positive):

```
anger : 44/45   -  .9777777777
contempt : 11/18   -  .6111111111
disgust : 54/59   -  .9152542372
fear : 19/25   -  .7600000000
happy : 65/69   -  .9420289855
neutral : 0/0   -  
sadness : 20/28   -  .7142857142
surprise : 82/83   -  .9879518072
Train hit rate: 295/327   -  .9021406727

```

#### SVM (linear kernel)

Training parameters:

* Face detector: `haarcascade_frontalface_alt.xml`, `rotation correction`
* Size: `48x48`
* Gabor kernels: `nwidths=2`, `nlambdas=5`, `nthetas=4`
* Multiclass mode: 1vsAllExt


Training parameters:

* Face detector: `haarcascade_frontalface_cbcl1.xml`, `rotation correction`
* Size: `48x48`
* Gabor kernels: `nwidths=2`, `nlambdas=5`, `nthetas=4`
* Multiclass mode: 1vsAllExt

```
Sadness
	sadness -> 1.00%
Neutral
	neutral -> 1.00%
Disgust
	disgust -> 1.00%
Anger
	anger -> 1.00%
Surprise
	surprise -> 1.00%
Fear
	fear -> 1.00%
Contempt
	contempt -> 1.00%
Happy
	happy -> 1.00%
```

Considerations:

* Better face detection
* Lower signal/noise ration in cropped face, so more generalization error

Training parameters:

* Face detector: `haarcascade_frontalface_cbcl1.xml`, `rotation correction`
* Size: `48x48`
* Gabor kernels: `nwidths=2`, `nlambdas=5`, `nthetas=4`
* Multiclass mode: 1vsAllExt

```
Sadness
	sadness -> 1.00%
Neutral
	neutral -> 1.00%
Disgust
	disgust -> 1.00%
Anger
	anger -> 1.00%
Surprise
	surprise -> 1.00%
Fear
	fear -> 1.00%
Contempt
	contempt -> 1.00%
Happy
	happy -> 1.00%
```

Considerations:

* More unstable face detection
* Higher signal/noise ration in cropped face, so less generalization error

## Validation

Due to the lack of availability of other dataset with respect to the time available for the project, we did __not__ performed formal _validation test_, so we cannot provide a validation error confusion matrix. Try it yourself. 

## Detection and Prediction

### Usage


Video gui:

  echo <VIDEOPATH> |./emotimevideo_cli FACEDETECTORXML (EXEDETECTORXML|none) WIDTH HEIGHT NWIDTHS NLAMBDAS NTHETAS (svm|ada) (TRAINEDCLASSIFIERSXML)+
  
Cam gui:

  ./emotimegui_cli FACEDETECTORXML (EXEDETECTORXML|none) WIDTH HEIGHT NWIDTHS NLAMBDAS NTHETAS (svm|ada) (TRAINEDCLASSIFIERSXML)+


## Further Development

* Validation dataset 
* Much more training samples, some emotions in Cohn-Kanade are under-sampled.
* Better GUI
* Better CLI tools

