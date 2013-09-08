/**
 *
 */


#include <iostream>
#include <sstream>
#include <string>
#include <opencv2/opencv.hpp>

#if defined(WIN32) || defined(_WIN32) 
#define PATH_SEPARATOR "\\" 
#else 
#define PATH_SEPARATOR "/" 
#endif 

using namespace std;
using namespace cv;


void help(){
	cout << "Usage:" << endl;
	cout << "   adatrain <trainFile> <outputFile> <nfeatures> [-p]" << endl;
	cout << "Parameters:" << endl;
	cout << "   <trainFile>   - File containing the training data " << endl;
	cout << "                   (CSV format, first column contains class, features in others)" << endl;
	cout << "   <outputFile>  - Output file where to store classifier" << endl;
	cout << "   <nfeatures>   - Number of features to consider" << endl;
	cout << "   -p            - Print train error and test error" << endl;
	cout << endl;
}
void banner(){
 	cout << "AdaTrain Utility:" << endl;
 	cout << "     Train an AdaBoost classifier "<< endl;
}

int main( int argc, const char* argv[] )
{
	if (argc < 3) {
		banner();
		help();
		cerr << "Error: missing parameters" << endl;
		return -3;
	} 
	const char * infile = argv[1];
	const char * outfile = argv[2];
	
	bool printErr=false;
	if (argc > 4){
		string par = string(argv[4]);
		string printpar = string("-p");
		if (printpar.compare(par)==0){
			printErr=true;
		}
	}

	try {
		int nfeat = abs(atoi(argv[3]));

		CvMLData cvml;
		cvml.read_csv(infile);
		cvml.set_response_idx(0);
		
		CvTrainTestSplit cvtts(200, true);
		cvml.set_train_test_split(&cvtts);
		
		CvBoost boost;
		
		boost.train(&cvml, CvBoostParams(CvBoost::REAL, nfeat, 0, 1, false, 0), false);
		boost.train(&cvml, CvBoostParams(CvBoost::REAL, nfeat, 0, 1, false, 0), false);
		
		std::vector<float> train_responses, test_responses;
		
		float fl1 = boost.calc_error(&cvml, CV_TRAIN_ERROR, &train_responses);
		float fl2 = boost.calc_error(&cvml, CV_TEST_ERROR, &test_responses);

		if (printErr){
			cout << "Train Error: " << fl1 << endl;
			cout << "Test Error:  " << fl2 << endl;
		}

		boost.save(outfile);
	}
	catch (int e) {
		cerr << "ERR: Exception #" << e << endl;
		return -e;
	}
	
}
