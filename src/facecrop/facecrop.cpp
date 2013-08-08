/*
 * facecrop.cpp
 *
 *  Created on: Aug 07, 2013
 *      Author: stk
 */

#include <stdlib.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "facedetector.h"

using namespace std;
using namespace cv;
using namespace facecrop;


void help(){
	cout << "Usage:" << endl;
	cout << "   facecrop <configFile> <inputImage> <outputImage>" << endl;
	cout << "Parameters:" << endl;
	cout << "   <configFile>   - OpenCV cascade classifier configuration file (Haar or LBP)" << endl;
	cout << "   <inputImage>   - Input image" << endl;
	cout << "   <outputImage>  - Output image" << endl;
	cout << endl;
}
void banner(){
 	cout << "FaceCrop Utility:" << endl;
 	cout << "     Crop faces from images" << endl;
}

int main( int argc, const char* argv[] )
{
	banner();
	if (argc != 4) {
		cerr << "Error: missing parameters";
		help();
	}  
	string config = string(argv[1]);
	string infile = string(argv[2]);
	string outfile = string(argv[3]);
	try {
		FaceDetector detector(config);
		Mat img = imread( infile, CV_LOAD_IMAGE_UNCHANGED);
		Rect face(0,0,0,0);

		detector.detect(img, face);

		if (face.x == 0 && face.y == 0 && face.width == 0 && face.height == 0){
			cout << "WARNING : No face detected in '" << infile << "'" << endl;
			return -2;
		}
		Mat cropped=img(face);

		imwrite( outfile, cropped );

	}
	catch (int e) {
		cerr << "Exception #" << e;
		return -1;
	}
	return 0;
}
