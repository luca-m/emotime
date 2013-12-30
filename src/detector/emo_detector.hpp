#ifndef _H_EMO_DETECTOR
#define _H_EMO_DETECTOR

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <limits>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

namespace emotime {
  
  enum Emotion {
    NEUTRAL=0,
    ANGER=1,
    CONTEMPT=2,
    DISGUST=3,
    FEAR=4,
    HAPPY=5,
    SADNESS=6,
    SURPRISE=7,
    OTHERS=8,
    UNKNOWN=9
  };

  /** 
   * Retrieve the string associated with an emotion
   * */ 
  string emotionStrings(Emotion emo);

  /**
   * Generic class for performing multi-class classification using binary classifiers.
   * At the moment implements only the voting scheme: return the class which receive much more votes.
   *
   * TODO: from map<string, pair<Emotion, D *> > 
   *       to map<string, pair<pair<Emotion,Emotion>, D *> > to support 1vs1 multiclass method
   * */
  template <class D>
  class EmoDetector {
    private:
    /**
     * Map containing all known detectors
     * */
    map<string, pair<Emotion, D *> > detectors;
    /**
     * Pointer to the prediction routine 
     * 
     * @param detector
     * @param frame
     * @return the predicted value or class (depend on the binary predictor used)
     * */
    protected:
    virtual float predict(D *detector, cv::Mat & frame)=0;
    
    public:
    /**
     * Initialize an EmoDetector
     *
     * @param prediction_routine the prediction routine to use when predicting a value with a detector of the specified type <D>
     * */
    EmoDetector(){
      detectors = map<string, pair<Emotion, D *> >();
    }
    /**
     * Initialize an EmoDetector
     *
     * @param prediction_routine the prediction routine to use when predicting a value with a detector of the specified type <D>
     * */
    EmoDetector( map<string, pair<Emotion, D *> > detmap ){
      #ifdef DEBUG
      cerr<<"DEBUG: adding "<<detmap.size()<<" detectors"<<endl;
      #endif
      detectors = detmap;
    }
    /**
     * Release an EmoDetector
     * */
    ~EmoDetector(){
      detectors.clear();
      //predict = NULL;
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
    void putDetector(string & name, Emotion emo, D & detector){
      detectors.insert( pair<string, pair<Emotion,D *> >(name, make_pair(emo, &detector)) );
    }
    /**
     * Get a detector by name
     * 
     * @param name
     * @return pointer to the detector. Raise an exception if not found. 
     * */
    D getDetector(string & name){
      if (!this->contains(name)){
        throw 10;
        cerr<<"ERR: could not find detector with name '"<<name<<"'"<<endl;
      } else {
        return (detectors[name].second);
      } 
    }
    /**
     * Obtain the list of detectors.
     * 
     * @return list of detectors
     * */
    vector<D> getDetectors(){
      vector<D>  dects = vector<D>();
      for( typename map<string, std::pair<Emotion, D *> >::iterator ii=detectors.begin(); ii!=detectors.end(); ++ii){
        dects.push_back(*(ii->second.second)); 
      }
      return dects;
    }
    /**
     * Obtain the list of detectors which can detect the specified emotion
     * 
     * @param emo
     * @return list of detectors
     * */
    vector<D> getDetectors(Emotion emo){
      vector<D> dects = vector<D>();
      for( typename map<string, pair<Emotion, D *> >::iterator ii=detectors.begin(); ii!=detectors.end(); ++ii){
        if ( (*ii).second.first==emo){
          dects.push_back(*(ii->second.second));
        } 
      }
      return dects;
    }

    /**
     *  @brief          Predict the emotion using the best-wins approach
     *
     *  @param[in]      frame The image to predict
     *
     *  @return         The prediction and its result
     *
     *  @details
     */
    pair<Emotion, float> predictBestWinsOneVsAll(cv::Mat & frame) {

      pair<Emotion, float> best(UNKNOWN, numeric_limits<float>::min());

      if (detectors.size() == 0) {
        #ifdef DEBUG
        cerr << "WARN: no detectors found! Unable to predict anything" << endl;
        #endif
        return make_pair(UNKNOWN, 0.0f);
      }

      for(typename map<string, pair<Emotion, D *> >::iterator ii =
          this->detectors.begin(); ii != this->detectors.end(); ++ii) {

        Emotion emo = ii->second.first;

        #ifdef DEBUG
        cerr << "DEBUG: detector " << ii->first << " is predicting ";
        #endif

        float prediction = predict(ii->second.second, frame);

        #ifdef DEBUG
        cerr << "(" << prediction << ")" << endl;
        #endif

        if (best.second < prediction) {
          best.first = emo;
          best.second = prediction;
        }
      }

      return best;
    }

    /**
     * Predict the class of the sample frame using a majority voting strategy.
     *
     * */
    pair<Emotion, float> predictMayorityOneVsAll(cv::Mat & frame){
      map<Emotion,float> votes; 

      if (detectors.size()==0){
        #ifdef DEBUG
        cerr<<"WARN: no detector found! Unable to predict anything"<<endl;
        #endif
        return make_pair(UNKNOWN,0.0f);
      }

      #ifdef DEBUG
      cerr<<"DEBUG: predictiong by voting"<<endl;
      #endif

      for( typename map<string, pair<Emotion, D *> >::iterator ii=detectors.begin(); ii!=detectors.end(); ++ii){
        Emotion emo = ii->second.first;
        #ifdef DEBUG
        cerr<<"DEBUG: detector "<<ii->first<<" is predicting ";
        #endif
        float prediction = predict(ii->second.second, frame);
        #ifdef DEBUG
        cerr<<"("<<prediction<<")"<<endl;
        #endif
        map<Emotion, float>::iterator it = votes.find(emo);  
        if (it==votes.end()){
          votes.insert(make_pair(emo, prediction));
        } else{
          it->second+=prediction;
        } 
      }
      
      pair<Emotion,float> max_pair = make_pair(UNKNOWN, numeric_limits<float>::min());
      for( map<Emotion, float>::iterator ii=votes.begin(); ii!=votes.end(); ++ii){
        if (ii->second > max_pair.second){
          max_pair.first=ii->first;
          max_pair.second=ii->second;
        }
      } 
      #ifdef DEBUG
      cerr<<"DEBUG: Most voted emotion is "<<emotionStrings(max_pair.first)<<" with score "<<max_pair.second<<endl;
      #endif
      return max_pair;
    }
    /**
     * Default multiclass prediction method
     * */
    pair<Emotion, float> predict(cv::Mat & frame){
      return predictMayorityOneVsAll(frame);
    }


  }; // end of EmoDetector
}

#endif
