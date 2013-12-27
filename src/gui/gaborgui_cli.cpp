#include "boost_emo_detector.h"
#include "matrix_io.h"
#include "gaborbank.h"
#include "facecrop.h"
#include "preprocessor.hpp"
#include "debuggui.hpp"
#include "capture.hpp"

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
	cout<<"   gaborgui_cli <FaceDetecXML> <video> "<<endl;
	cout<<"Parameters:"<<endl;
	cout<<"   <faceDetectConf>   - OpenCV cascade classifier configuration file (Haar or LBP) for face detection"<<endl;
	cout<<"   <video>            - Video file "<<endl;
	cout<<endl;
}
void banner() {
	cout<<"GaborGui Utility:"<<endl;
	cout<<"     GUI for gabor parameter tuning"<<endl;
}

int main( int argc, const char *argv[] ) {
  if (argc < 3) {
		banner();
		help();
		cerr<<"ERR: missing parameters"<<endl;
		return -3;
	}
	const char *faceDetConfig = argv[1];
  const char *infile = argv[2];
	try {
    Image capture=Image(string(infile), true/*grayscale*/);
    GaborGui gui=GaborGui(&capture, string(faceDetConfig));
    gui.run();
	} catch (int e) {
		cerr<<"ERR: Exception #"<<e<<endl;
		return -e;
	}
  return 0;
}
