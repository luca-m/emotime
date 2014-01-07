#ifndef _H_EMO_DETECTOR
#define _H_EMO_DETECTOR

#include "matrix_io.h"
#include "string_utils.h"

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

    /// Map containing all known detectors
    map<string, pair<Emotion, D *> > detectors;

    /// Detectors for generic approaches (each detector matches one or more emotion)
    map<string, pair<vector<Emotion>, D *> > detectors_ext;


    /**
     *  @brief          Initialize the detectors in simple mode (one detector, one emotion)
     *
     *  @param[in]      detmap A map to name -> (emotion, detector)
     *
     */
    void init(const map<string, pair<Emotion, D *> >& detmap) {
      #ifdef DEBUG
      cerr << "DEBUG: adding " << detmap.size() << " detectors" << endl;
      #endif
      detectors = detmap;

      for(typename map<string, std::pair<Emotion, D *> >::const_iterator ii = detmap.begin();
          ii != detmap.end(); ++ii) {
        vector<Emotion> ev;
        ev.push_back(ii->second.first);
        this->detectors_ext.insert(make_pair(ii->first, make_pair(ev, ii->second.second)));
      }
    }

    /**
     *  @brief          Initialize the detectors in extended mode (one detector multiple emotion)
     *
     *  @param[in]      detmap_ext A map to name -> (vector<emotion>, detector)
     *
     */
    void init(const map<string, pair<vector<Emotion>, D *> >& detmap_ext) {
      #ifdef DEBUG
      cerr << "DEBUG: adding " << detmap_ext.size() << " detectors" << endl;
      #endif
      this->detectors_ext = detmap_ext;

      for(typename map<string, std::pair<vector<Emotion>, D *> >::const_iterator ii = detmap_ext.begin();
          ii != detmap_ext.end(); ++ii) {
        vector<Emotion> emv = ii->second.first;
        if(emv.size() == 1) {
          this->detectors_ext.insert(make_pair(ii->first, make_pair(emv[0], ii->second.second)));
        }
      }
    }

    protected:

    /**
     * Pointer to the prediction routine
     *
     * @param detector
     * @param frame
     * @return the predicted value or class (depend on the binary predictor used)
     *
     */
    virtual float predict(D *detector, cv::Mat & frame)=0;

    public:
    /**
     * Initialize an EmoDetector
     *
     * @param prediction_routine the prediction routine to use when predicting a value with a detector of the specified type <D>
     * */
    EmoDetector(){
      map<string, pair<Emotion, D *> > detmap;
      init(detmap);
    }

    /**
     *  @brief          Initialize the detectors in simple mode (one detector, one emotion)
     *
     *  @param[in]      detmap A map to name -> (emotion, detector)
     *
     */
    EmoDetector( map<string, pair<Emotion, D *> > detmap ){
      init(detmap);
    }


    /**
     *  @brief          Initialize the detectors in extended mode (one detector multiple emotion)
     *
     *  @param[in]      detmap_ext A map to name -> (vector<emotion>, detector)
     *
     */
    EmoDetector(map<string, pair<vector<Emotion>, D *> > detmap_ext) {
      init(detmap_ext);
    }


    /**
     *  @brief          Creates an emodetector from a vector of classifiers path
     *
     *  @param[in]      classifier_paths  A vector containing the classifiers path
     *  @param[in]      initialized_classifiers  A vector containing the initialized classifiers
     *
     *  @details        Path must be in the format: emop1_emop2_emopn_vs_emon1_emon2_emonm.xml
     *                  Where emop* is the emotion recognized and emon* is the emotion not recognized.
     */
    EmoDetector(vector<string> classifier_paths, vector<D*> initialized_classifiers) {

      map<string, pair<vector<Emotion>, D*> > classifiers;

      for(size_t i = 0; i < classifier_paths.size(); i++) {

        string clpath = classifier_paths.at(i);
        D* cvD = initialized_classifiers.at(i);

        string fname = matrix_io_fileBaseName(clpath);
        Emotion emo = UNKNOWN;

        vector<string> emotions_list = split_string(fname, "_");
        vector<Emotion> fin_emo_list;
        fin_emo_list.reserve(emotions_list.size());
        string label = "";

        for(vector<string>::iterator it = emotions_list.begin(); it !=
            emotions_list.end(); ++it) {
          emo = UNKNOWN;
          if (*it == "vs") {
            break;
          } else if (*it == emotionStrings(NEUTRAL)) {
            emo = NEUTRAL;
          } else if (*it == emotionStrings(ANGER)) {
            emo = ANGER;
          } else if (*it == emotionStrings(CONTEMPT)) {
            emo = CONTEMPT;
          } else if (*it == emotionStrings(DISGUST)) {
            emo = DISGUST;
          } else if (*it == emotionStrings(FEAR)) {
            emo = FEAR;
          } else if (*it == emotionStrings(HAPPY)) {
            emo = HAPPY;
          } else if (*it == emotionStrings(SADNESS)) {
            emo = SADNESS;
          } else if (*it == emotionStrings(SURPRISE)) {
            emo = SURPRISE;
          }
          if(emo != UNKNOWN) {
            if(label.size() > 0) {
              label.append("_");
            }
            label.append(emotionStrings(emo));
            fin_emo_list.push_back(emo);
          }
        }
        pair<vector<Emotion>, D*> value(fin_emo_list, cvD);
        pair<string, pair<vector<Emotion>, D*> > entry(label, value);
        classifiers.insert(entry);
      }

      init(classifiers);
    }

    /**
     * Release an EmoDetector
     * */
    ~EmoDetector(){
      detectors.clear();
      detectors_ext.clear();
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

      vector<Emotion> ev;
      ev.push_back(emo);
      this->detectors_ext.insert(make_pair(name, make_pair(ev, &detector)));
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
     *  @brief          Predict the emotion using the extended classification.
     *
     *  @param[in]      frame The image to predict
     *
     *  @return         The prediction and its result
     *
     *  @details
     */
    pair<Emotion, float> predictVotingOneVsAllExt(cv::Mat & frame) {

      map<Emotion,float> votes;

      if (detectors_ext.size() == 0) {
        #ifdef DEBUG
        cerr << "WARN: no detectors found! Unable to predict anything" << endl;
        #endif
        return make_pair(UNKNOWN, 0.0f);
      }

      votes.insert(make_pair(NEUTRAL, 0.f));
      votes.insert(make_pair(CONTEMPT , 0.f));
      votes.insert(make_pair(DISGUST, 0.f));
      votes.insert(make_pair(SADNESS, 0.f));
      votes.insert(make_pair(ANGER, 0.f));
      votes.insert(make_pair(HAPPY, 0.f));
      votes.insert(make_pair(FEAR, 0.f));
      votes.insert(make_pair(UNKNOWN, 0.f));

      for(typename map<string, pair<vector<Emotion>, D *> >::iterator ii =
          this->detectors_ext.begin(); ii != this->detectors_ext.end(); ++ii) {

        vector<Emotion> emo = ii->second.first; // detected emotions

        #ifdef DEBUG
        cerr << "DEBUG: detector " << ii->first << " is predicting ";
        #endif
        float prediction = predict(ii->second.second, frame);
        #ifdef DEBUG
        cerr << "(" << prediction << ")" << endl;
        #endif

        for(vector<Emotion>::iterator emo_it = emo.begin(); emo_it != emo.end(); ++emo_it) {
          map<Emotion, float>::iterator it = votes.find(*emo_it);
          if (it == votes.end()) {
            votes.insert(make_pair(*emo_it, prediction));
          } else{
            if (prediction > 0.5) {
              it->second += 0.5; //1.0;
            } else {
              for(map<Emotion,float>::iterator votes_it = votes.begin(); votes_it != votes.end(); ++votes_it) {
                vector<Emotion>::iterator e_it = find(emo.begin(), emo.end(), votes_it->first);
                if (e_it == emo.end()) {
                  // if I dont find emotion in detected emotion
                  votes_it->second+=1.0;
                }
              }
              //it->second -= 0.0;
            }
          }
        }
      }

      pair<Emotion,float> max_pair = make_pair(UNKNOWN, numeric_limits<float>::min());

      for( map<Emotion, float>::iterator ii = votes.begin(); ii != votes.end();
          ++ii) {
        if (ii->second > max_pair.second) {
          max_pair.first = ii->first;
          max_pair.second = ii->second;
        }
      }

      #ifdef DEBUG
      cerr << "DEBUG: Most voted emotion is " << emotionStrings(max_pair.first)
        << " with score " << max_pair.second << endl;
      #endif

      return max_pair;
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
    virtual pair<Emotion, float> predict(cv::Mat & frame){
      return predictMayorityOneVsAll(frame);
    }


  }; // end of EmoDetector
}

#endif
