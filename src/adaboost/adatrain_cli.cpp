/**
 *
 */
#include <iostream>
#include <sstream>

#include "adatrain.h"

void help() {
	cout << "Usage:" << endl;
	cout << "   adatrain_cli <trainFile> <outputFile> <nfeatures> [-p]" << endl;
	cout << "Parameters:" << endl;
	cout << "   <trainFile>   - File containing the training data " << endl;
	cout << "                   (CSV format, first column contains class, features in others)" << endl;
	cout << "   <outputFile>  - Output file where to store classifier" << endl;
	cout << "   <nfeatures>   - Number of features to consider" << endl;
	cout << "   -p            - Print train error and test error" << endl;
	cout << endl;
}
void banner() {
	cout << "AdaTrain Utility:" << endl;
	cout << "     Train an AdaBoost classifier " << endl;
}
int main( int argc, const char *argv[] ) {
	if (argc < 3) {
		banner();
		help();
		cerr << "Error: missing parameters" << endl;
		return -3;
	}
	const char *infile = argv[1];
	const char *outfile = argv[2];

	bool printErr = false;
	if (argc > 4) {
		string par = string(argv[4]);
		string printpar = string("-p");
		if (printpar.compare(par) == 0) {
			printErr = true;
		}
	}

	try {
		int nfeat = abs(atoi(argv[3]));
		CvBoost boost;

		adatrain_trainCSV(boost, infile, nfeat, printErr);

		boost.save(outfile);
	} catch (int e) {
		cerr << "ERR: Exception #" << e << endl;
		return -e;
	}

}
