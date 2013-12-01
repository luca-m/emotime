/**
 *
 */
#include "registration.h"

#ifdef DEBUG
#include <iostream>
#endif 

using namespace std;

void registerImage(cv::Mat & src, cv::Mat & dst){
  #ifdef DEBUG
  cerr<<"DEBUG: registering image (equalizing)"<<endl;
  #endif 
  cv::equalizeHist(src, dst);
 	return;
}
