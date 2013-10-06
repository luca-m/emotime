/*
 * facecrop_cli.cpp
 *
 *  Created on: Aug 07, 2013
 *      Author: stk
 */

#include <stdlib.h>
#include <iostream>

#include "facecrop.h"

using namespace std; 
using namespace cv;
using namespace facecrop;

void help(){
	cout << "Usage:" << endl;
	cout << "   facecrop_cli <configFile> <inputImage> <outputImage> [--register]" << endl;
	cout << "Parameters:" << endl;
	cout << "   <configFile>   - OpenCV cascade classifier configuration file (Haar or LBP)" << endl;
	cout << "   <inputImage>   - Input image" << endl;
	cout << "   <outputImage>  - Output image" << endl;
	cout << "   --register     - Register the image (at the moment: equalization)" << endl;
	cout << endl;
}
void banner(){
 	cout << "FaceCrop Utility:" << endl;
 	cout << "     Crop faces from images" << endl;
}

int main( int argc, const char* argv[] )
{
	
	if (argc < 4) {
		banner();
		help();
		cerr << "ERR: missing parameters" << endl;
		return -3;
	}  
	string config = string(argv[1]);
	string infile = string(argv[2]);
	string outfile = string(argv[3]);
	bool regist=false;

	if (argc==5){
		string par=string(argv[4]);
		string reg=string("--register");
		if (reg.compare(par)== 0){
			regist=true;
		}
	}

	try {
		FaceDetector detector(config);
		Mat img = imread( infile, CV_LOAD_IMAGE_GRAYSCALE);
		Mat cropped=img;
		facecrop_cropFace( detector, img, cropped, regist );
		imwrite( outfile, cropped );
	}
	catch (int e) {
		cerr << "ERR: Exception #" << e << endl;
		return -e;
	}
	return 0;
}