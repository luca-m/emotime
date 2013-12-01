#include "emo_detector.hpp"

/** 
* Retrieve the string associated with an emotion
* */ 
string emotime::emotionStrings(Emotion emo){
      switch (emo){ 
        case NEUTRAL: return string("neutral");
        case ANGER: return string("anger");
        case CONTEMPT: return string("contempt");
        case DISGUST: return string("disgust");
        case FEAR: return string("fear");
        case HAPPY: return string("happy");
        case SADNESS: return string("sadness");
        case SURPRISE: return string("surprise");
        default: return string("unknown");
      }
};

