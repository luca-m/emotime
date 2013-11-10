#ifndef _H_MATRIX_IO
#define _H_MATRIX_IO

#include <opencv2/opencv.hpp>
#include <string>

#if defined(WIN32) || defined(_WIN32) 
#define PATH_SEPARATOR "\\" 
#else 
#define PATH_SEPARATOR "/" 
#endif 

#define YMLEXT "yml"
#define XMLEXT "xml"

/**
 * Retrieve file extension
 * 
 * @param file
 * */
std::string matrix_io_fileExt(std::string & file);
/**
 * Retrieve file name without extensione and path
 *
 * @param file
 * */
std::string matrix_io_fileName(std::string & file);
/**
 * Load matrix from an image or from a data file (xml,yml)
 *
 * @param filePath Add ".xml" or ".yml" to file path for using data file 
 * */
cv::Mat matrix_io_load( std::string & filePath);
/**
 * Save matrix to an image or to a data file (xml,yml). 
 * 
 *
 * @param mat 
 * @param filePath Add ".xml" or ".yml" to file path for using data file 
 * */
bool matrix_io_save( cv::Mat & mat, std::string & filePath);

#endif // _H_MATRIX_IO
