/**
 *
 * @file    gaborbank_cli.cpp
 * @author  Daniele Bellavista (daniele.bellavista@studio.unibo.it)
 * @date    12/04/2013 10:33:42 AM
 * @brief   CLI interface for filtering an image using Gabor's Filters.
 *
 * @details 
 *
 */

#include <iostream>
#include <sstream>
#include <string>

#include "GaborBank.h"
#include "matrix_io.h"

using namespace emotime;

using std::cout;
using std::cerr;
using std::endl;
using std::string;

using cv::Mat;

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
  cout << "   gaborbank_cli <imageWidth> <imageHeight> <nwidths> <nlambdas> <nthetas> <inputImage> <outputFile>" << endl;
  cout << "Parameters:" << endl;
  cout << "   <imageWidth>    - Width of the image, the input image will be scaled" << endl;
  cout << "   <imageHeight>   - Height of the image, the input image will be scaled" << endl;
  cout << "   <nwidths>       - " << endl;
  cout << "   <nlambdas>      - " << endl;
  cout << "   <nthetas>       - " << endl;
  cout << "   <inputImage>    - Input image" << endl;
  cout << "   <outputFile>    - Output file  where to store filtered images" << endl;
  cout << endl;
}

void banner() {
   cout << "GaborBank Utility:" << endl;
   cout << "     Filter with a bank of Gabor filters with different " <<  endl;
   cout << "     orientations and frequencies. (Gabor magnitude)" << endl;
}

int main( int argc, const char* argv[] ){
  if (argc < 5) {
    banner();
    help();
    cerr << "ERR: missing parameters" << endl;
    return -3;
  }
    unsigned int width = abs(atoi(argv[1]));
    unsigned int height = abs(atoi(argv[2]));
    double nwidths = (atof(argv[3]));
    double nlambdas = (atof(argv[4]));
    double nthetas = (atof(argv[5]));
    string infile(argv[6]);
    string outfile(argv[7]);

    Mat img = matrix_io_load(infile);
    Mat scaled;
    resize(img, scaled, cv::Size(width,height), 0, 0, CV_INTER_AREA);
    img.release();

    GaborBank bank;
    bank.fill_gabor_bank((double) nwidths, (double) nlambdas, (double) nthetas);
    Mat dest = bank.filter_image(scaled);
    matrix_io_save(dest, outfile);

    scaled.release();
    dest.release();
}
