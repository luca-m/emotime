/**
 *
 * */

#include <opencv2/opencv.hpp>

using namespace cv;

/**
 * Return the set of index of the features which are used as first split variables
 * in the trained decision trees.
 *  
 * @param boost boosted 
 * */
std::set<unsigned int> featselect_firstSplit( CvBoost & boost );
/**
 * Return the set of index of the features which have importance major than zero in the trained decision trees.
 * Taken from "mushroom.cpp" example file in OpenCV sources.
 *  
 * @param boost boosted 
 * */
std::set<unsigned int> featselect_varImportance( CvBoost & boost ){
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
 * Save selected features index to file.
 *
 * @param selected
 * @param file_path
 * @param append
 * */
bool featselect_save( std::set<unsigned int> & selected, const char * file_path, bool append);
/**
 * Merge current selected features with those ones contained in the file path specified.
 *
 * @param selected
 * @param file_path
 * */
bool featselect_merge( std::set<unsigned int> & selected, const char * file_path);
