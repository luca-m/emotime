/**
 *
 * @file    svmtrain_cli.cpp
 * @author  Daniele Bellavista (daniele.bellavista@studio.unibo.it)
 * @date    12/04/2013 10:33:42 AM
 * @brief   CLI interface for training a Support Vector Machine
 *
 * @details 
 *
 */

#include "svmtrain.h"
#include "csv.h"

#include <iostream>
#include <sstream>
#include <string>

using namespace cv;
using namespace std;

/**
 *  @brief          Prints the CLI banner
 *
 */
void banner();

/**
 *  @brief          Prints the CLI help
 *
 */
void help();

void help() {
  cout << "Usage:" << endl;
  cout << "   svmtrain_cli <trainFile> <outputFile> [<nfeatures>]" << endl;
  cout << "Parameters:" << endl;
	cout << "   <trainFile>   - File containing the training data " << endl;
	cout << "                   (CSV format, first column contains class, features in others)" << endl;
	cout << "   <outputFile>  - Output file where to store classifier" << endl;
	cout << "   <nfeatures>   - Number of features to consider. Note: this option automatically enable" << endl;
	cout << "                   the \"read samples from CSV\" mode." << endl;
  cout << endl;
}

void banner() {
  cout << "SVMTrain Utility:" << endl;
	cout << "   Train an SVM classifier." << endl;
	cout << "   Train a SVM classifier using data specified in a CSV file." << endl;
	cout << "   NOTE: the CSV file may contain sample data directly (aka read samples from CSV mode)" << endl;
	cout << "         or also contains path of the file where to find samples (images or OpenCV yml)" << endl;
  cout << "" << endl;
}

int main( int argc, const char* argv[] )
{

  if (argc < 3) {
    banner();
    help();
    cerr << "ERR: missing parameters" << endl;
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
		CvSVM svm;
    if (samplesInCsv){
		  svmtrain_trainCvMLData(svm, infile, nfeat, true);
    } else{
      std::vector<string> files;
      std::vector<int> classes;
      csv_load_traindata(infile, files, classes);
      svmtrain_trainManual(svm, files, classes);
    }
    svm.save(outfile);
	} catch (int e) {
		cerr << "ERR: Exception #" << e << endl;
		return -e;
	}
}
