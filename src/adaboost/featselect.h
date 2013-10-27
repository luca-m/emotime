/**
 *
 * */

#include <opencv2/opencv.hpp>

using namespace cv;

/**
 * Return the set of index of the features which are used as first split variables
 * in the decision trees trained.
 *  
 * @param boost boosted 
 * */
std::set<unsigned int> featselect_firstSplit( CvBoost & boost );
/**
 * Filter the specified vector of features.
 *
 * @param feat_vec feature vector (NxM) 
 * @param selected selected features index set
 * @return A Nx1 matrix containing the selected features.
 * */
cv::Mat featselect_select(cv::Mat & feat_vec, std::set<unsigned int> & selected );
/**
 *
 * @param file_path 
 * */
std::set<unsigned int> featselect_load(const char * file_path);
/**
 *
 * @param file_path
 * */
bool featselect_save( std::set<unsigned int> & selected, const char * file_path);

