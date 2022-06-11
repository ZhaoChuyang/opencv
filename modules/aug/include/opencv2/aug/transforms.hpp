#ifndef OPENCV_AUG_TRANSFORMS_HPP
#define OPENCV_AUG_TRANSFORMS_HPP

#include <opencv2/core.hpp>

namespace cv{
    CV_EXPORTS_W void randomCrop(InputArray src, OutputArray dst, const Size& sz, const Vec4i& padding=Vec4i(), bool pad_if_need=false, int fill=0, int padding_mode=0);
}

#endif
