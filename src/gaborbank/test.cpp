/**
 *
 * @file    test.cpp
 * @author  Daniele Bellavista (daniele.bellavista@studio.unibo.it)
 * @date    12/31/2013 04:48:16 PM
 * @brief   
 *
 * @details 
 *
 */

#include <opencv2/opencv.hpp>
#include "gaborbank.h"

/**
 *  @brief          
 *
 *  @param[in]      argc The arguments count
 *  @param[in]      argv The program arguments
 *
 *  @return         the exit code
 *
 *  @details        
 */
int main(int argc, char *argv[]) {
//  cv::namedWindow("Pippo", cv::WINDOW_NORMAL);
//  cv::namedWindow("Pluto", cv::WINDOW_NORMAL);

  cv::Mat gabor_mine, gabor_real, gabor_im, gabor_lol;

  double sigma = 16.0;
  double theta = 0.0;
  double lambda = 1.0;
  double gamma = 0.5;
  double psi = 0.0;

  gabor_real = gaborbank_getGaborKernel(cv::Size(100, 100), sigma, theta, lambda, gamma, psi, CV_32F, true);
  gabor_im = gaborbank_getGaborKernel(cv::Size(100, 100), sigma, theta, lambda, gamma, psi, CV_32F, true);

  gabor_lol = cv::getGaborKernel(cv::Size(100, 100), sigma, theta, lambda, gamma, psi, CV_32F);

  imshow("Pippo", gabor_mine);
  imshow("Pluto", gabor_lol);

  while(1) cv::waitKey(0);
}				/* ----------  end of function main  ---------- */
