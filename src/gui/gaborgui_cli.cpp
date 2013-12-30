/**
 *
 * */
#include "debuggui.hpp"
#include "capture.hpp"
#include "boost_emo_detector.h"
#include "matrix_io.h"
#include "gaborbank.h"

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <map>
#include <iostream>

using namespace std;
using namespace cv;
using namespace emotime;

void help() {
	cout<<"Usage:"<<endl;
	cout<<"   gaborgui_cli <faceDetecXML> <eyeDetectXML> <video> "<<endl;
	cout<<"Parameters:"<<endl;
	cout<<"   <faceDetectXML>    - OpenCV cascade classifier configuration file (Haar or LBP) for face detection"<<endl;
	cout<<"   <eyeDetectXML>     - OpenCV cascade classifier configuration file (Haar or LBP) for eye detection"<<endl;
	cout<<"   <video>            - Video file "<<endl;
	cout<<endl;
}
void banner() {
	cout<<"GaborGui Utility:"<<endl;
	cout<<"     GUI for gabor parameter tuning"<<endl;
}

int main( int argc, const char *argv[] ) {
  if (argc<4) {
		banner();
		help();
		cerr<<"ERR: missing parameters"<<endl;
		return -3;
	}
	string faceDetConfig = string(argv[1]);
	string eyeDetConfig = string(argv[2]);
  string infile = string(argv[3]);
	try {
    Image capture=Image(infile, true/*grayscale*/);
    GaborGui gui=GaborGui(&capture, faceDetConfig, eyeDetConfig);
    gui.run();
	} catch (int e) {
		cerr<<"ERR: Exception #"<<e<<endl;
		return -e;
	}
  return 0;
}
