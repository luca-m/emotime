/**
 *
 */
#include "adatrain.h"

bool adaboost_trainCSV(CvBoost & boost, const char * csvFilePath, int nFeatures, bool printErr ){
		CvMLData cvml;
		cvml.read_csv(csvFilePath);
		cvml.set_response_idx(0);
		CvTrainTestSplit cvtts(ADATRAIN_TRAINING_PORTION, true);
		cvml.set_train_test_split(&cvtts);

		boost.train(&cvml, CvBoostParams(CvBoost::REAL, nFeatures, 0, 1, false, 0), false);

		std::vector<float> train_responses, test_responses;
		float fl1 = boost.calc_error(&cvml, CV_TRAIN_ERROR, &train_responses);
		float fl2 = boost.calc_error(&cvml, CV_TEST_ERROR, &test_responses);

		if (printErr){
			cout << "INFO: Train Error: " << fl1 << endl;
			cout << "INFO: Test Error:  " << fl2 << endl;
		}
		return true;
}
