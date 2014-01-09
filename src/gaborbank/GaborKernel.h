/**
 * @class    GaborKernel
 * @author
 * @date
 *
 * @brief   Representation of a gabor kernel
 *
 * @details
 *
 */

#ifndef  __GABORKERNEL_INC__
#define  __GABORKERNEL_INC__

#include <opencv2/opencv.hpp>

namespace emotime {

  class GaborKernel {

    public:

      /**
       *  @brief          Creates a gabor kernel specifying real and imaginary parts
       *
       *  @param[in]      r   The real part
       *  @param[in]      i   The imaginary part
       *
       */
      GaborKernel(cv::Mat r, cv::Mat i) {
        r.copyTo(real);
        i.copyTo(imag);
      }

      ~GaborKernel() {
        real.release();
        imag.release();
      }

      /**
       *  @brief          Return the real part
       *
       *  @return   The real part of the filter
       */
      cv::Mat getReal() {
        return real;
      }

      /**
       *  @brief          Return the imaginary part
       *
       *  @return   The imaginary part of the filter
       */
      cv::Mat getImag() {
        return imag;
      }

    private:
      /// Real part
      cv::Mat real;
      /// Immaginary part
      cv::Mat imag;
  };

}

#endif   /* ----- #ifndef __GABORKERNEL_INC__  ----- */
