/*
 * facecrop_cli.cpp
 */

#include <stdlib.h>
#include <iostream>

#include "facedetector.h"
#include "matrix_io.h"

using namespace std; 
using namespace cv;
using namespace emotime;

void help(){
	cout<<"Usage:"<<endl;
	cout<<"   facecrop_cli <FaceConfigFile> <EyeConfigFile> <inputImage> <outputImage>"<<endl;
	cout<<"Parameters:"<<endl;
	cout<<"   <FaceConfigFile>    - OpenCV cascade classifier configuration file (Haar or LBP)"<<endl;
	cout<<"   <EyesConfigFile>    - OpenCV cascade classifier configuration file (Haar or LBP)"<<endl;
	cout<<"   <inputImage>        - Input image"<<endl;
	cout<<"   <outputImage>       - Output image"<<endl;
	cout<<endl;
}
void banner(){
 	cout<<"FaceCrop Utility:"<<endl;
 	cout<<"     Crop faces from images"<<endl;
}

int main( int argc, const char* argv[] ){
	if (argc<5) {
		banner();
		help();
		cerr<<"ERR: missing parameters"<<endl;
		return -3;
	}  
	string config_f = string(argv[1]);
	string config_e = string(argv[2]);
	string infile = string(argv[3]);
	string outfile = string(argv[4]);
	try {
		FaceDetector detector(config_f, config_e);
		Mat img = matrix_io_load(infile);
		Mat cropped;
		if (detector.detect(img, cropped)){
		  matrix_io_save(cropped, outfile); 
    }else{
		  cerr<<"ERR: no face found.."<<endl;
    }
	}
	catch (int e) {
		cerr<<"ERR: Exception #"<<e<<endl;
		return -e;
	}
	return 0;
}
