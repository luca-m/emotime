/**
 * @file    EmoDetector.cpp
 * @date    01/10/2014 01:12:27 AM
 * @brief
 *
 * @details
 *
 */

#include "matrix_io.h"
#include "string_utils.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <limits>

#include <opencv2/opencv.hpp>

#include "EmoDetector.h"

using std::pair;
using std::map;
using std::string;
using std::vector;
using std::make_pair;

using std::numeric_limits;

using std::cerr;
using std::endl;

using cv::Mat;

namespace emotime {

  string emotionStrings(Emotion emo){
    switch (emo) {
      case NEUTRAL: return string("neutral");
      case ANGER: return string("anger");
      case CONTEMPT: return string("contempt");
      case DISGUST: return string("disgust");
      case FEAR: return string("fear");
      case HAPPY: return string("happy");
      case SADNESS: return string("sadness");
      case SURPRISE: return string("surprise");
      case OTHERS: return string("others");
      default: return string("unknown");
    }
  };

  void EmoDetector::init(const std::map<std::string, std::pair<std::vector<Emotion>, Classifier*> >& detmap_ext) {
    this->detectors_ext = detmap_ext;

    for(map<string, pair<vector<Emotion>, Classifier*> >::const_iterator ii = detmap_ext.begin();
        ii != detmap_ext.end(); ++ii) {
      vector<Emotion> emv = ii->second.first;
      this->detectors_ext.insert(make_pair(ii->first, make_pair(emv, ii->second.second)));
    }
  }

  EmoDetector::EmoDetector() {

  }

  EmoDetector::EmoDetector(std::map<std::string, std::pair<std::vector<Emotion>, Classifier*> >& detmap_ext) {
    init(detmap_ext);
  }

  EmoDetector::~EmoDetector() {
    for(map<string, pair<vector<Emotion>, Classifier*> >::const_iterator ii = this->detectors_ext.begin();
        ii != this->detectors_ext.end(); ++ii) {
      delete ii->second.second;
    }
    detectors_ext.clear();
  }

  void EmoDetector::init(std::vector<std::string>& classifier_paths) {
    map<string, pair<vector<Emotion>, Classifier*> > classifiers;

    for(size_t i = 0; i < classifier_paths.size(); i++) {

      string clpath = classifier_paths[i];
      Classifier* cvD = this->createClassifier();
      cvD->load(clpath);

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
      pair<vector<Emotion>, Classifier*> value(fin_emo_list, cvD);
      pair<string, pair<vector<Emotion>, Classifier*> > entry(label, value);
      classifiers.insert(entry);
    }

    init(classifiers);
  }

  pair<Emotion, float> EmoDetector::predictVotingOneVsAllExt(cv::Mat& frame, std::map<Emotion,float> &votes) {

    if (detectors_ext.size() == 0) {
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

    for(map<string, pair<vector<Emotion>, Classifier*> >::iterator ii =
        this->detectors_ext.begin(); ii != this->detectors_ext.end(); ++ii) {

      vector<Emotion> emo = ii->second.first; // detected emotions
      Classifier* cl = ii->second.second;

      float prediction = cl->predict(frame);

      for(vector<Emotion>::iterator emo_it = emo.begin(); emo_it != emo.end(); ++emo_it) {
        map<Emotion, float>::iterator it = votes.find(*emo_it);
        if (it == votes.end()) {
          votes.insert(make_pair(*emo_it, prediction));
        } else{
          if (prediction > 0.5) {
            it->second += 0.5; //1.0;
          } else {
            //for(map<Emotion,float>::iterator votes_it = votes.begin(); votes_it != votes.end(); ++votes_it) {
            //  vector<Emotion>::iterator e_it = find(emo.begin(), emo.end(), votes_it->first);
            //  if (e_it == emo.end()) {
            //    // if I dont find emotion in detected emotion
            //    votes_it->second+=1.0;
            //  }
            //}
            it->second -= 0.0;
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

    return max_pair;
  }

  pair<Emotion, float> EmoDetector::predictBestWinsOneVsAll(cv::Mat& frame) {

    pair<Emotion, float> best(UNKNOWN, numeric_limits<float>::min());

    if (detectors_ext.size() == 0) {
      return make_pair(UNKNOWN, 0.0f);
    }

    for (map<string, pair<vector<Emotion>, Classifier*> >::iterator ii =
        this->detectors_ext.begin(); ii != this->detectors_ext.end(); ++ii) {

      if (ii->second.first.size() != 1) {
        continue;
      }
      Emotion emo = ii->second.first[0];
      Classifier* cl = ii->second.second;

      float prediction = cl->predict(frame);

      if (best.second < prediction) {
        best.first = emo;
        best.second = prediction;
      }
    }

    return best;
  }

  pair<Emotion, float> EmoDetector::predictMayorityOneVsAll(cv::Mat& frame, std::map<Emotion,float> &votes){

    if (detectors_ext.size() == 0) {
      return make_pair(UNKNOWN, 0.0f);
    }

    for (map<string, pair<vector<Emotion>, Classifier*> >::iterator ii =
        this->detectors_ext.begin(); ii != this->detectors_ext.end(); ++ii) {

      if (ii->second.first.size() != 1) {
        continue;
      }
      Emotion emo = ii->second.first[0];
      Classifier* cl = ii->second.second;

      float prediction = cl->predict(frame);

      map<Emotion, float>::iterator it = votes.find(emo);
      if (it == votes.end()) {
        votes.insert(make_pair(emo, prediction));
      } else {
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
    return max_pair;
  }

  pair<Emotion, float> EmoDetector::predict(cv::Mat& frame, std::map<Emotion,float> &votes) {
    return predictMayorityOneVsAll(frame, votes);
  }
}
