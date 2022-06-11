#include <iostream>
#include "precomp.hpp"
#include <opencv2/highgui.hpp>

namespace cv{

    static void getRandomCropParams(int h, int w, int th, int tw, int* x, int* y);

    // NOTE: cv::randomCrop or randomCrop?
    void randomCrop(InputArray _src, OutputArray _dst, const Size& sz, const Vec4i& padding, bool pad_if_need, int fill, int padding_mode){
        // FIXME: whether the size of dst should be (src.cols+left+right, src.rows+top+bottom)
        Mat src = _src.getMat();

        _dst.create(sz.height, sz.width, src.type());
        Mat dst = _dst.getMat();
//        src.copyTo(dst);
//        Mat tmp = src.clone();
        Mat tmp;
        src.copyTo(tmp);

        if(padding != Vec4i()){
            copyMakeBorder(tmp, tmp, padding[0], padding[1], padding[2], padding[3], padding_mode, fill);
        }

        // NOTE: make sure src.rows == src.size().height and src.cols = src.size().width
        // pad the height if needed
        if(pad_if_need && src.rows < sz.height){
            Vec4i _padding = {sz.height - src.rows, sz.height - src.rows, 0, 0};
            copyMakeBorder(src, src, _padding[0], _padding[1], _padding[2], _padding[3], padding_mode, fill);
        }
        // pad the width if needed
        if(pad_if_need && src.cols < sz.width){
            Vec4i _padding = {0, 0, sz.width - src.cols, sz.width - src.cols};
            copyMakeBorder(src, src, _padding[0], _padding[1], _padding[2], _padding[3], padding_mode, fill);
        }

        int x, y;
        getRandomCropParams(src.rows, src.cols, sz.height, sz.width, &x, &y);
        std::cout << x << " " << y << std::endl;
        Mat RoI(src, Rect(x, y, sz.width, sz.height));
        RoI.copyTo(dst);
        std::cout << src.rows << " " << src.cols << std::endl;
//        imshow("lena.png", dst);
//        waitKey(0);
        std::cout << dst.rows << " " << dst.cols << std::endl;
    }

    static void getRandomCropParams(int h, int w, int th, int tw, int* x, int* y){
        if(h+1 < th || w+1 < tw){
            // FIXME: throw exception
        }
        if(h == th && w == tw){
            (*x) = 0;
            (*y) = 0;
            return;
        }
        RNG& rng = theRNG();
        (*x) = rng.uniform(0, w-tw+1);
        (*y) = rng.uniform(0, h-th+1);
    }
}