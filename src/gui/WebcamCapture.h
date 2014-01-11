/**
 * WebcamCapture.h
 * Copyright (C) Luca Mella <luca.mella@studio.unibo.it>
 *
 * Distributed under terms of the CC-BY-NC license.
 */

#ifndef WEBCAMCAPTURE_H
#define WEBCAMCAPTURE_H

#include "VideoCapture.h"

namespace emotime {
/**
   * @class    WebcamCapture
   *
   * @brief   Capture implementation using a Webcam
   *
   * @details
   *
   */
  class WebcamCapture : public VideoCapture {

    public:

      /**
       * @brief          WebcamCapture costructor
       *
       * @param[in]      to_grey If true, each photogram will be converted to greyscale
       *
       */
      WebcamCapture(bool to_grey) : VideoCapture(0, to_grey){}

    protected:

    private:

  };


}


#endif /* !WEBCAMCAPTURE_H */

