/**
 *
 */
#include "adatrain.h"
#include "../utils/csv.h"

#include <vector>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

void help() {
	cout<<"Usage:" << endl;
	cout<<"   adatrain_cli <trainFile> <outputFile> [<nfeatures>]" << endl;
  cout<<"Parameters:" << endl;
	cout<<"   <trainFile>   - File containing the training data " << endl;
	cout<<"                   (CSV format, first column contains class, features in others)" << endl;
	cout<<"   <outputFile>  - Output file where to store classifier" << endl;
	cout<<"   <nfeatures>   - Number of features to consider. Note: this option automatically enable" << endl;
	cout<<"                   the \"read samples from CSV\" mode." << endl;
	cout<<endl;
}
void banner() {
	cout<<"AdaTrain Utility" << endl;
	cout<<"   Train an AdaBoost classifier." << endl;
	cout<<"   Train a boosted tree using data specified in a CSV file." << endl;
	cout<<"   NOTE: the CSV file may contain sample data directly (aka read samples from CSV mode)" << endl;
	cout<<"         or also contains path of the file where to find samples (images or OpenCV yml)" << endl;
  cout<<""<<endl;
}
int main( int argc, const char *argv[] ) {
	if (argc < 3) {
		banner();
		help();
		cerr<<"ERR: missing parameters"<<endl;
		return -3;
	}
	const char * infile=argv[1];
	const char * outfile=argv[2];
	bool samplesInCsv=false;
	int nfeat=0;
  if (argc > 3) {
		nfeat = abs(atoi(argv[3]));
		samplesInCsv=true;
	}
	try {
		CvBoost boost;
    if (samplesInCsv){
		  adatrain_trainCvMLData(boost, infile, nfeat, true);
    } else{
      std::vector<string> files;
      std::vector<int> classes; 
      csv_load_traindata(infile, files, classes); 
      adatrain_trainManual(boost, files, classes); 
    }
		boost.save(outfile);
	} catch (int e) {
		cerr<<"ERR: Exception #"<<e<<endl;
		return -e;
	}

}
