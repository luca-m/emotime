
#include "gabor.h"

void getGaborBank(std::vector<cv::Mat> & bank) {
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
	return;
}
