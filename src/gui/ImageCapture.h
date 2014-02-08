/**
 *
 * @file    ImageCapture.h
 * @brief   Defines ImageCapture
 *
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
   */
  class ImageCapture: public ACapture {

    public:

      /**
       * @brief Load an image from file.
       *
       * @param[in] infile  The file to load
       * @param[in] grayScale Determinates if the image should be converted to
       * grayscale.
       *
       */
      ImageCapture(string infile, bool grayScale);

      /**
       *  @brief          Implementation of ACaputre::isReady
       *
       *  @return         Returns true if the image was loaded correctly
       *
       */
      bool isReady();

    protected:

      /**
       *  @brief          Fill \p frm with the image.
       *
       *  @param[in,out]      frm The frame to fill
       *
       *  @return         False if the image wasn't successfully loaded.
       *
       */
      bool extractFrame(Mat & frm);

    private:

      /// Current image
      Mat image;

  };

}

#endif /* !IMAGECAPTURE_H */

