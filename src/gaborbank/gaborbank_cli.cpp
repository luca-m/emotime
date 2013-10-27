/**
 *
 * */
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
	cout << "   gaborbank_cli <imageWidth> <imageHeight> <inputImage> <outputFile>" << endl;
	cout << "Parameters:" << endl;
	cout << "   <imageWidth>    - Width of the image, the input image will be scaled" << endl;
	cout << "   <imageHeight>   - Height of the image, the input image will be scaled" << endl;
	cout << "   <inputImage>    - Input image" << endl;
	cout << "   <outputFile>    - Output file  where to store filtered images" << endl;
	cout << endl;
}
void banner(){
 	cout << "GaborBank Utility:" << endl;
 	cout << "     Filter with a bank of Gabor filters with different "<< endl;
 	cout << "     orientations and frequencies. (Gabor magnitude)" << endl;
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

		vector<GaborKern*> bank;
		gaborbank_getGaborBank(bank);
    cout << "INFO: filtering with "<< bank.size() << " filters" << endl;
		Mat dest = gaborbank_filterImage(scaled, bank);

		stringstream ss;
		ss << outfolder ;
		string outfile = ss.str();
    cout << "INFO: saving to " << outfile << endl; 
		imwrite( outfile, dest );
    dest.release();
	}
	catch (int e) {
		cerr << "ERR: Exception #" << e << endl;
		return -e;
	}
}
