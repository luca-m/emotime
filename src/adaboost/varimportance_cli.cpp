//#include <cv.h>       // opencv general include file
//#include <ml.h>		  // opencv machine learning include file

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv; // OpenCV API is in the C++ "cv" namespace
using namespace std;

// prints out the relative importance of the variables (i.e. attributes) used
// for decision tree classification

// Based on the mushroom.cpp example from OpenCV 1.0

int print_variable_importance(CvBoost & boost) {
    CvSeq *predictors = boost.get_weak_predictors();
    CvBoostTree * predictor;
    cout << "weak predictors " << predictors->total << endl;
    for (int i = 0; i < predictors->total; i++) {
         predictor = *CV_SEQ_ELEM(predictors, CvBoostTree*, i);
         const CvDTreeNode * node = predictor->get_root();
         CvDTreeSplit * split = node->split; 
         //cout<<" predictor "<<i<<" root var_idx "<<split->var_idx<<" quality="<<split->quality<<endl;
         
         const CvMat * var_importance = predictor->get_var_importance();
         for (int j = 0; j < var_importance->cols * var_importance->rows; j++) {
            double val = var_importance->data.db[j];
            if (val > 0) {
                cout << "predictor "<< i <<" pixel "<<j<< " importance " <<val << endl;
            }
         }
         //var_importance->release();
         //break;
    }
	return 1;
}

int main( int argc, char** argv )
{
	if (argc == 2)
	{
		CvBoost boost;
		boost.load(argv[1]);
		if (print_variable_importance(boost)){
			return 0; // all OK
		} else {
			return -1; // all not OK
		}

    } else {
	  cout << "usage: "<< argv[0] <<" decision_tree_filename.xml" << endl;
    return -1;
    }
}
