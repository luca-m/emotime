/**
 * ImageCapture.h
 * Copyright (C) Luca Mella <luca.mella@studio.unibo.it>
 *
 * Distributed under terms of the CC-BY-NC license.
 */

#ifndef IMAGECAPTURE_H
#define IMAGECAPTURE_H

#include "ACapture.h"

namespace emotime{

  /**
   * @class ImageCapture
   *
   * @brief Capture frames from a single image.
   * 
   * @details
   *
   * */
  class ImageCapture: public ACapture {
    
    public:

      /**
       * @brief Load an image from file.
       *
       * @param[in]
       * @param[in]
       *
       * */
      ImageCapture(string infile, bool grayScale);
      
      bool isReady();

    protected:

      bool extractFrame(Mat & frm);
    
    private:
      
      /// Current image 
      Mat image;
  
  };

}

#endif /* !IMAGECAPTURE_H */

