#ifndef OPENCV_AUG_FUNCTIONAL_HPP
#define OPENCV_AUG_FUNCTIONAL_HPP
#include <opencv2/core.hpp>
//#include <iostream>

namespace cv {

    void adjust_brightness(Mat& img, double brightness_factor){
        int channels = img.channels();
        if(channels != 1 and channels != 3){
            CV_Error(Error::BadNumChannels, "Only support images with 1 or 3 channels");
        }
        img.convertTo()

    }

    void adjust_contrast(Mat& img, double contrast_factor);

    void adjust_saturation(Mat& img, double saturation_factor);

    void adjust_hue(Mat& img, double hue_factor);

};


#endif