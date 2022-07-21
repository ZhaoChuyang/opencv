#ifndef OPENCV_AUG_FUNCTIONAL_HPP
#define OPENCV_AUG_FUNCTIONAL_HPP
#include <opencv2/core.hpp>
//#include <iostream>

namespace cv {

//    void blend(Mat& img1, Mat& img2, float ratio){
//
//    }

    void adjust_brightness(Mat& img, float brightness_factor){
        CV_Assert(brightness_factor >= 0);

        int channels = img.channels();
        if(channels != 1 and channels != 3){
            CV_Error(Error::BadNumChannels, "Only support images with 1 or 3 channels");
        }

        // NOTE: Can substitute for-loop with matrix multiplication for better efficiency?
        int nc = channels * img.cols;
        for(int j=0; j<img.rows; j++){
            uchar* data = img.ptr<uchar>(j);
            for(int i=0; i<nc; i++){
                data[i] = static_cast<uchar>( data[i] * brightness_factor);
            }
        }
    }

    void adjust_contrast(Mat& img, double contrast_factor);

    void adjust_saturation(Mat& img, double saturation_factor);

    void adjust_hue(Mat& img, double hue_factor);

};


#endif