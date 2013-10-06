/**
*
*/
#include "gaborbank.h"

#include <iostream>

void gaborbank_getGaborBank(std::vector<cv::Mat> & bank) {
	double _sigma;	/// Sigma of the Gaussian envelope
	double _theta;  /// Orientation of the normal to the parallel stripes of the Gabor function
	double _lambda;	/// Wavelength of sinusoidal factor
	double _gamma;  /// Spatial aspect ratio (ellipticity of the support of the Gabor function)
	double _psi;	/// Phase offset
	_gamma = 1.0;
	_sigma = 2.0;
	_lambda = 1.0;
	for (_lambda = 4.0; _lambda < 16.0; _lambda += 2.5) {
		cv::Size kernelSize(_lambda,_lambda);
		for (_theta = 0.0; _theta < (2.0 * CV_PI); _theta += (CV_PI / 4.0)) {
			bank.push_back(
					cv::getGaborKernel(kernelSize, (double) _sigma,
							(double) _theta, (double) _lambda,
							(double) _gamma));
		}
	}
}

void gaborbank_getFilteredImgSize( cv::Mat src, std::vector<cv::Mat> & bank , cv::Size & outSize ){
	// The output image will contain all the filtered image vertically stacked. 
	outSize.height = src.rows * bank.size();
	outSize.width = src.cols;
}

void gaborbank_filterImage( cv::Mat & src, std::vector<cv::Mat> & bank, cv::Mat & dest ){
		cv::Size size;
		unsigned int i=0;	
		unsigned int j=0;
		unsigned int k=0;
		size.height= src.rows;
		size.width= src.cols;
		for (k=0; k < bank.size(); k++) {
			cv::Mat kern = bank.at(k);
			cv::Mat filtered = cv::Mat(size, CV_8UC1);
			std::cout << "DEBUG: Filtering with filer #"<< k <<" w"<<size.width<<" h"<<size.height<<" - "<<(k*size.height)<<std::endl;
			filter2D(src,filtered,0,kern);
			// Write all the filtered image vertically stacked.
			for (i=0; i< size.width ; i ++ ){
				for (j=0; j < size.height; j++ ){
					dest.at<unsigned char>(i, j + (k*size.height))= filtered.at<unsigned char>(i,j);
				}
			}
		}
}
