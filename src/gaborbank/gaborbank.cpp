/**
*
*/
#include "gaborbank.h"

using namespace cv;
using namespace std;

/**
 * Gabor Kernel generator.
 *
 * Taken from OpenCV implementation and modified for output both Real and Imaginary part of the Gabor kernel.
 *
 * See http://en.wikipedia.org/wiki/Gabor_filter for parameter detail
 *
 * */
Mat gaborbank_getGaborKernel(Size ksize, double sigma, double theta, double lambd, double gamma, double psi, int ktype, bool real) {
    double sigma_x = sigma;
    double sigma_y = sigma/gamma;
    int nstds = 3;
    int xmin, xmax, ymin, ymax;
    double c = cos(theta), s = sin(theta);
    if( ksize.width > 0 )
        xmax = ksize.width/2;
    else
        xmax = cvRound(std::max(fabs(nstds*sigma_x*c), fabs(nstds*sigma_y*s)));
    if( ksize.height > 0 )
        ymax = ksize.height/2;
    else
        ymax = cvRound(std::max(fabs(nstds*sigma_x*s), fabs(nstds*sigma_y*c)));
    xmin = -xmax;
    ymin = -ymax;
    CV_Assert( ktype == CV_32F || ktype == CV_64F );
    Mat kernel(ymax-ymin+1, xmax-xmin+1, ktype);
    double scale = 1;
    double ex = -0.5/(sigma_x*sigma_x);
    double ey = -0.5/(sigma_y*sigma_y);
    double cscale = CV_PI*2/lambd;
    for( int y = ymin; y <= ymax; y++ ) {
        for( int x = xmin; x <= xmax; x++ )
        {
            double xr = x*c + y*s;
            double yr = -x*s + y*c;
            double v = 0.0; 
            if (real) {
              v=scale*exp(ex*xr*xr + ey*yr*yr)*cos(cscale*xr + psi); 
            }
            else {
              v=scale*exp(ex*xr*xr + ey*yr*yr)*sin(cscale*xr + psi);
            }
            if( ktype == CV_32F )
                kernel.at<float>(ymax - y, xmax - x) = (float)v;
            else
                kernel.at<double>(ymax - y, xmax - x) = v;
        }
    }
    return kernel;
}
/**
 * Prepare a bank of gabor filters
 *
 * See "Dynamics of facial expression extracted automatically from video" section 2
 * See "Distortion Invariant Object Recognition in the Dynamic Link Architecture" section 3.H
 * See "Jesper Juul Henriksen Thesis" at page 22  <\a href="http://covil.sdu.dk/publications/jespermaster07.pdf">  
 * For parameters value detail.
 *
 */
void gaborbank_getGaborBank(std::vector<struct GaborKern *> & bank) {
  #ifdef DEBUG
  cout << "DEBUG: generating gabor bank" << endl; 
  #endif
  double _sigma;        /// Sigma of the Gaussian envelope
  double _theta;        /// Orientation of the normal to the parallel stripes of the Gabor function
  double _lambda;       /// Wavelength of sinusoidal factor
  double _gamma;        /// Spatial aspect ratio (ellipticity of the support of the Gabor function)
  double _psi;          /// Phase offset
  _gamma = 1;
  _sigma = 4.0;
  _lambda = sqrt(2);
  _psi=0;
  int fwidth=7;
  for (fwidth=7;fwidth<17;fwidth+=(17-7)/3) {
    for (_lambda = CV_PI/32.0; _lambda < CV_PI/2.; _lambda += (CV_PI/2.0-CV_PI/32.0)/5.0 ) {
      cv::Size kernelSize(fwidth,fwidth);
      for (_theta = 0.0; _theta < (CV_PI); _theta += (CV_PI/8.0)) {
         struct GaborKern * kern = new struct GaborKern;
         kern->real = gaborbank_getGaborKernel(kernelSize, _sigma, _theta, _lambda, _gamma, _psi, CV_32F, true).clone();
         kern->imag = gaborbank_getGaborKernel(kernelSize, _sigma, _theta, _lambda, _gamma, _psi, CV_32F, false).clone();
         bank.push_back(kern);
         #ifdef DEBUG
         imshow("real",real);
         imshow("imag",imag);
         waitKey(0);
         #endif
      }
    }
  }
}
/**
 *
 *
 * */
Size gaborbank_getFilteredImgSize( cv::Mat & src, std::vector<struct GaborKern *> & bank ){
  #ifdef DEBUG
  cout << "DEBUG: calculating correct size for filtered image." << endl; 
  #endif
	// The output image will contain all the filtered image vertically stacked.
  Size s = Size(0,0); 
	s.height = src.rows * bank.size();
	s.width = src.cols;
  return s;
}
/**
 *
 *
 * */
Mat gaborbank_filterImage( cv::Mat & src, std::vector<struct GaborKern *> & bank ){
  #ifdef DEBUG
  cout << "INFO: filtering.." << endl; 
  #endif
	Size size(0,0);
	unsigned int i=0;	
	unsigned int j=0;
	unsigned int k=0;
  Size s = gaborbank_getFilteredImgSize(src,bank);
  Mat * dest = new Mat(s, CV_32F);
  Mat image;
	size.height=src.rows;
	size.width=src.cols;
  src.convertTo(image, CV_32F); 
	for (k=0; k < bank.size(); k++) {
		GaborKern * gk= bank.at(k);
    Mat real = gk->real;
    Mat imag = gk->imag;
		Mat freal = Mat(size,CV_32F);
		Mat fimag = Mat(size,CV_32F);
		Mat magn  = Mat(size,CV_32F);
		filter2D(src, freal, CV_32F, real);
		filter2D(src, fimag, CV_32F, imag);
    pow(freal,2,freal);
    pow(fimag,2,fimag);
    add(fimag,freal,magn);
    sqrt(magn,magn);
		// Write all the filtered image vertically stacked.
    // TODO: use image1.copyTo ( Mat ( display, Rect(y,x,h,w) ) )
		for (i=0; i< size.height ; i ++ ){
			for (j=0; j < size.width; j++ ){
				dest->at<float>(i+(k*size.height), j) = magn.at<float>(i,j);
			}
		}
	}
  //delete s;
  return *dest;
}

