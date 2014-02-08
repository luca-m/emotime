/**
 *
 * @file    matrix_io.h
 * @brief   IO utilities for opencv matrix.
 *
 */

#ifndef _H_MATRIX_IO
#define _H_MATRIX_IO

#include <opencv2/opencv.hpp>
#include <string>

#if defined(WIN32) || defined(_WIN32)
/// System dependent path separator
#define PATH_SEPARATOR "\\"
#else
/// System dependent path separator
#define PATH_SEPARATOR "/"
#endif

/// YML extension
#define YMLEXT "yml"
/// XML extension
#define XMLEXT "xml"

/**
 * Retrieve file extension
 *
 * @param[in] file The filename
 *
 * @return The file extension
 *
 */
std::string matrix_io_fileExt(std::string& file);

/**
 * Retrieve file name without extension and path
 *
 * @param[in] file The filename
 *
 * @return The file name without extension and path
 *
 */
std::string matrix_io_fileName(std::string& file);

/**
 * Retrieve the file basename of a filepath
 *
 * @param[in] file The filename
 *
 * @return The base name
 *
 */
std::string matrix_io_fileBaseName(std::string& file);

/**
 * Load matrix from an image or from a data file (xml,yml)
 *
 * @param[in] filePath The filename
 *
 * @details If \p filePath has as extension YMLEXT or XMLEXT, then the file is
 * treated as data file.
 *
 * @return The loaded matrix
 *
 */
cv::Mat matrix_io_load(std::string& filePath);

/**
 * Save matrix to an image or to a data file (xml,yml).
 *
 * @param[in] mat The matrix to save
 * @param[in] filePath The destination file. If its extension is YMLEXT or
 *                     XMLEXT, then the file is saved as data file.
 *
 * @return True if the save operation was successful
 *
 */
bool matrix_io_save(cv::Mat& mat, std::string& filePath);

#endif // _H_MATRIX_IO
