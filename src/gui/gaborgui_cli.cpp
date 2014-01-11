/**
 *
 * */
#include "GaborGui.hpp"
#include "ImageCapture.h"
#include "matrix_io.h"

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
	cout<<"   gaborgui_cli <image> "<<endl;
	cout<<"Parameters:"<<endl;
	cout<<"   <video>            - Image file "<<endl;
	cout<<endl;
}
void banner() {
	cout<<"GaborGui Utility:"<<endl;
	cout<<"     GUI for gabor parameter tuning"<<endl;
}

int main( int argc, const char *argv[] ) {
  if (argc<1) {
		banner();
		help();
		cerr<<"ERR: missing parameters"<<endl;
		return -3;
	}
  string infile = string(argv[1]);
	try {
    ImageCapture capture=ImageCapture(infile, true/*grayscale*/);
    GaborGui gui=GaborGui(&capture);
    gui.run();
	} catch (int e) {
		cerr<<"ERR: Exception #"<<e<<endl;
		return -e;
	}
  return 0;
}
