/**
 *
 */


#include <iostream>
#include <sstream>

#include "adapredict.h"

using namespace std;
using namespace cv;

void help() {
	cout << "Usage:" << endl;
	cout << "   adapredict_cli <adaConfig> <image> <nfeatures> " << endl;
	cout << "Parameters:" << endl;
	cout << "   <adaConfig>   - OpenCV XML configuration file containig " << endl;
	cout << "                   the classifier structure." << endl;
	cout << "   <image>       - The input image" << endl;
	cout << endl;
}
void banner() {
	cout << "AdaPredict Utility:" << endl;
	cout << "     Predict the an AdaBoost classifier " << endl;
}

int main( int argc, const char *argv[] ) {
	if (argc < 2) {
		banner();
		help();
		cerr << "Error: missing parameters" << endl;
		return -3;
	}
	const char *config = argv[1];
	const char *infile = argv[2];

	bool printErr = false;
	if (argc > 4) {
		string par = string(argv[3]);
		string printpar = string("-p");
		if (printpar.compare(par) == 0) {
			printErr = true;
		}
	}

	try {		
		Mat img = imread( infile, CV_LOAD_IMAGE_GRAYSCALE);
		CvBoost boost;
		boost.load(config);

		float prediction = adapredict_predictNoPreprocess( boost, img );

		cout << "Precicted value:  " << prediction << endl; 

	} catch (int e) {
		cerr << "ERR: Exception #" << e << endl;
		return -e;
	}

}
