/**
 * @class    EmotimeGUI
 * @author   Daniele Bellavista (daniele.bellavista@studio.unibo.it)
 * @date    12/31/2013 10:53:55 AM
 *
 * @brief   Emotime GUI: capture from a webcam and predict the emotion
 *
 * @details
 *
 */

#include "agui.hpp"
#include "capture.hpp"

#ifndef  __EMOTIME_GUI_INC__
#define  __EMOTIME_GUI_INC__

namespace emotime {

  class EmotimeGUI : public AGui {
    public:

      EmotimeGUI(FacePreProcessor* fp, EmoDetector* detect, int fps) :
        AGui(new Webcam(true), fp, detect, fps, "Emotime!") {

      }

      ~EmotimeGUI() {
        delete this->capture;
      }

    protected:

      bool newFrame(Mat& frame, pair<Emotion, float> prediction) {
        Mat copy;
        frame.copyTo(copy);
        stringstream ss;
        ss << "Emotion=" << emotionStrings(prediction.first) << ", Score=" << prediction.second;
        string osd = ss.str();

        cv::putText(frame, osd.c_str(), Point(80,60), FONT_HERSHEY_SIMPLEX, 0.7, Scalar::all(0));
        // QT only
        //displayOverlay(mainWinTitle.c_str(), osd.c_str(), 2000);
        return true;
      }

    private:

  }; /* -----  end of class EmotimeGUI  ----- */

}

#endif   /* ----- #ifndef __EMOTIME_GUI_INC__  ----- */
