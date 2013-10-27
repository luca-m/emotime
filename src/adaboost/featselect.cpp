/**
 *
 * */

#include "featselect.h"
#include <iostream>
#include <fstream>

#ifdef DEBUG
#include <iostream>
#endif

using namespace cv;
using namespace std;

std::set<unsigned int> featselect_firstSplit( CvBoost & boost ){
    set<unsigned int> * selected = new set<unsigned int>();  
    CvBoostTree * predictor;
    CvSeq *predictors = boost.get_weak_predictors();
    if (predictors){
        #ifdef DEBUG
        cout << "DEBUG: Number of weak predictors=" << predictors->total << endl;
        #endif
        for (int i = 0; i < predictors->total; i++) {
          predictor = *CV_SEQ_ELEM(predictors, CvBoostTree*, i);
          const CvDTreeNode * node = predictor->get_root();
          CvDTreeSplit * split = node->split; 
          #ifdef DEBUG
          cout<<"DEBUG: Predictor="<<i<<", Root var_idx="<<split->var_idx<<", Quality="<<split->quality<<endl;
          #endif
          selected->insert(split->var_idx);
          //const Mat * var_importance;
          //var_importance = (Mat*) predictor->get_var_importance();
          //for (int j = 0; j < var_importance->cols * var_importance->rows; j++) {
          //   double val = var_importance->at<double>(0,j);
          //  break; 
          //}
        }
   }
	return *selected;
}

cv::Mat featselect_select(cv::Mat & feat_vec, std::set<unsigned int> & selected ){
  Mat * feat_selected = new Mat( selected.size(), 1, CV_32FC1); 
  unsigned int i=0;
  for (set<unsigned int>::iterator it=selected.begin(); it!=selected.end(); ++it){
    unsigned int idx = *it;
    if (idx < feat_vec.rows){
      feat_selected->at<float>(i,0)=feat_vec.at<float>( ( (int) idx/feat_vec.rows) , ( (int) idx%feat_vec.cols) );
      i++;
    }
  } 
  return *feat_selected;
}

std::set<unsigned int> featselect_load(const char * file_path){
  set<unsigned int> * selected = new set<unsigned int>();
  try {
    std::ifstream file;
    unsigned int idx;
    file.open(file_path);
    while ( file >> idx ){
     selected->insert(idx); 
    }
    file.close();
  } catch (ifstream::failure e){
    cerr << "ERR: cant read file " << file_path << endl;
  }
  return *selected;
}

bool featselect_save( std::set<unsigned int> & selected, const char * file_path, bool append){
  try{
    std::ofstream file;
    file.open(file_path,  ios::out | (append?ios::app:0) );
    for (set<unsigned int>::iterator it=selected.begin(); it!=selected.end(); ++it){
      unsigned int idx = *it;
      file << idx << endl;
    }
    file.close();
    return true;
  } catch (ofstream::failure e){
    cerr << "ERR: cant write file " << file_path << endl;
    return false; 
  }
}

bool featselect_merge( std::set<unsigned int> & selected, const char * file_path){
  set<unsigned int> read = featselect_load(file_path);
  for (set<unsigned int>::iterator it=selected.begin(); it!=selected.end(); ++it){
    unsigned int idx = *it;
    read.insert(idx);
  }
  bool res = featselect_save( read, file_path, false);
  read.clear();
  return res;
}


