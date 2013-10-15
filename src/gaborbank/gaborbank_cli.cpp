/*
 * gaborbank.cpp
 *
 *  Created on: Aug 07, 2013
 *      Author: stk
 */

#include <iostream>
#include <sstream>
#include <string>

#include "gaborbank.h"

#if defined(WIN32) || defined(_WIN32) 
#define PATH_SEPARATOR "\\" 
#else 
#define PATH_SEPARATOR "/" 
#endif 

using namespace std;
using namespace cv;


void help(){
	cout << "Usage:" << endl;
	cout << "   gaborbank_cli <imageWidth> <imageHeight> <inputImage> <outputFolder>" << endl;
	cout << "Parameters:" << endl;
	cout << "   <imageWidth>    - Width of the image, the input image will be scaled" << endl;
	cout << "   <imageHeight>   - Height of the image, the input image will be scaled" << endl;
	cout << "   <inputImage>    - Input image" << endl;
	cout << "   <outputFolder>  - Output folder where to store filtered images" << endl;
	cout << endl;
}
void banner(){
 	cout << "GaborBank Utility:" << endl;
 	cout << "     Filter with a bank of Gabor filters with different "<< endl;
 	cout << "     orientations and frequencies. (the real part only)" << endl;
}

int main( int argc, const char* argv[] )
{
	
	if (argc < 5) {
		banner();
		help();
		cerr << "ERR: missing parameters" << endl;
		return -3;
	} 
	try {
		unsigned int width = abs(atoi(argv[1]));
		unsigned int height = abs(atoi(argv[2]));
		string infile = string(argv[3]);
		string outfolder = string(argv[4]);
	
		Mat img = imread(infile, CV_LOAD_IMAGE_GRAYSCALE);
		Mat scaled;
		resize( img, scaled, Size(width,height), 0, 0, CV_INTER_AREA );

		vector<Mat> bank;

		gaborbank_getGaborBank(bank);

		Size suggSize;

		gaborbank_getFilteredImgSize(scaled,bank,suggSize);

		Mat * dest = new Mat( suggSize, CV_8UC1 );

		gaborbank_filterImage(scaled, bank, *dest);

		stringstream ss;
		ss << outfolder << PATH_SEPARATOR << "filtered" << ".png";
		string outfile = ss.str();
		imwrite( outfile, *dest );

	}
	catch (int e) {
		cerr << "ERR: Exception #" << e << endl;
		return -e;
	}
}
