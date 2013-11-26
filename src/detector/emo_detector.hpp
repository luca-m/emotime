#ifndef _H_EMO_DETECTOR
#define _H_EMO_DETECTOR

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <limits>

using namespace std;
using namespace cv;

namespace emotime {
  
  enum Emotions {
    Neutral = 0,
    Anger = 1,
    Contempt = 2,
    Disgust= 3,
    Fear= 4,
    Happy= 5,
    Sadness= 6,
    Surprise= 7
  };

  /**
   * Generic class for performing multi-class classification using binary classifiers.
   * At the moment implements only the voting scheme: return the class which receive much more votes.
   *
   * */
  template <class D>
  class EmoDetector {
    private:
    /**
     * Map containing all known detectors
     * */
    map<string, pair<Emotions, D> > detectors;
    /**
     * Pointer to the prediction routine 
     * 
     * @param detector
     * @param frame
     * @return the predicted value or class (depend on the binary predictor used)
     * */
    float (*predict) (D & detector, cv::Mat & frame);
    
    public:
    /**
     * Initialize an EmoDetector
     *
     * @param prediction_routine the prediction routine to use when predicting a value with a detector of the specified type <D>
     * */
    EmoDetector( float (*prediction_routine) (D &, Mat &) ){
      detectors = map<string, pair<Emotions, D> >();
      predict = prediction_routine;
    }
    /**
     * Initialize an EmoDetector
     *
     * @param prediction_routine the prediction routine to use when predicting a value with a detector of the specified type <D>
     * */
    EmoDetector( map<string, pair<Emotions, D> > detmap, float (*prediction_routine) (D &, Mat &) ){
      detectors = detmap;
      predict = prediction_routine;
    }
    /**
     * Release an EmoDetector
     * */
    ~EmoDetector(){
      detectors.clear();
      predict = NULL;
    }
    /**
     * Return true if 
     * @return 
     * */
    bool contains(string & name){
      return detectors.find(name)!=detectors.end();
    }
    /**
     *  Add a detector
     *  
     *  @param name
     *  @param emo 
     *  @param detector
     * */
    void putDetector(string & name, Emotions emo, D & detector){
      detectors.insert( pair<string, pair<Emotions,D> >(name, make_pair(emo,detector)) );
    }
    /**
     * Get a detector by name
     * 
     * @param name
     * @return pointer to the detector. Raise an exception if not found. 
     * */
    D getDetector(string & name){
      if (!contains(name)){
        throw 10;
        cerr<<"ERR: could not find detector with name '"<<name<<"'"<<endl;
      } else {
        return & detectors[name].second;
      } 
    }
    /**
     * Obtain the list of detectors.
     * 
     * @return list of detectors
     * */
    vector<D> getDetectors(){
      vector<D>  dects = vector<D>();
      for( map<string,pair<Emotions,D> >::iterator ii=detectors.begin(); ii!=detectors.end(); ++ii){
        dects.push_back( (*ii).second.second ); 
      }
      return dects;
    }
    /**
     * Obtain the list of detectors which can detect the specified emotion
     * 
     * @param emo
     * @return list of detectors
     * */
    vector<D> getDetectors(Emotions emo){
      vector<D> dects = vector<D>();
      for( map<string,pair<Emotions,D> >::iterator ii=detectors.begin(); ii!=detectors.end(); ++ii){
        if ( (*ii).second.first==emo){
          dects.push_back( (*ii).second.second );
        } 
      }
      return dects;
    }
    /**
     * Predict the class of the sample frame using a majority voting strategy.
     *
     * */
    pair<Emotions, float> predictMayorityOneVsAll(cv::Mat & frame){
      map<Emotions,float> votes;

      for( map<string,pair<Emotions,D> >::iterator ii=detectors.begin(); ii!=detectors.end(); ++ii){
        Emotions emo = ii->second.first;
        float prediction = predict(ii->second.second, frame);
        map<Emotions, float>::iterator it = votes.find(emo);  
        if (it==votes.end()){
          votes.insert(make_pair(emo, prediction));
        } else{
          it->second+=prediction;
        } 
      }
      
      pair<Emotions,float> max_pair = make_pair(Neutral, numeric_limits<float>::min());
      for(map<Emotions,float>::iterator ii=detectors.begin(); ii!=detectors.end(); ++ii){
        if (ii->second > max_pair.second){
          max_pair.first=ii->first;
          max_pair.second=ii->second;
        }
      } 
      return max_pair;
    }

  };
}

#endif
