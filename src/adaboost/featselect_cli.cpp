/**
 *
 * */
#include <iostream>
#include <sstream>

#include "featselect.h"

using namespace std;
using namespace cv;

void help() {
	cout << "Usage:" << endl;
	cout << "   feateselect_cli <adaConfig> [<outputfile>]" << endl;
	cout << "Parameters:" << endl;
	cout << "   <adaConfig>   - OpenCV XML configuration file containig " << endl;
	cout << "                   the trained boosted decision trees." << endl;
	cout << "   <outputFile>  - Output file where to store the list of selected feature index." << endl;
	cout << "                   NOTE: selected feat will be merged to the feature indexes already contained in the output file." << endl;
	cout << endl;
}
void banner() {
	cout << "AdaBoost Feature Selection Utility:" << endl;
	cout << "     Select important features from a trained boosted classifier." << endl;
}

int main( int argc, const char *argv[] ) {
	bool preprocess=false;
  if (argc<2) {
		banner();
		help();
		cerr << "ERR: missing parameters" << endl;
		return -3;
  }
  bool save=false;
  const char *outfile;
  const char *config = argv[1];

  if (argc==3){
    outfile=argv[2];
    save=true;
  }

	try {		
		CvBoost boost;
		boost.load(config);

    set<unsigned int> feats_idx = featselect_varImportance(boost);  //featselect_firstSplit( boost ); 
    for (set<unsigned int>::iterator it=feats_idx.begin(); it!=feats_idx.end(); ++it){
      cout << *it << endl;
    }
    if (save){
        featselect_merge(feats_idx, outfile);
    }
    feats_idx.clear();
  } catch (int e) {
		cerr << "ERR: Exception #" << e << endl;
		return -e;
	}
  return 0;
}
