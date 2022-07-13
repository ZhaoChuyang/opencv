#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/aug.hpp>
#include <iostream>
#include <vector>


int main(int argv, char **argc) {
    const char* filename = argc[1];
    cv::Mat src = cv::imread(filename);
    cv::Mat dst;

    cv::Pad pad(cv::Vec4i(100, 100, 100, 100), 255);
    pad.call(src, dst);
    cv::imshow("lena.png", dst);
    cv::waitKey(0);
//    cv::randomCrop(src, dst, cv::Size(100, 100));
//    cv::imshow("lena.png", dst);
//    cv::waitKey(0);
//    cv::Compose transforms(all_transforms);
//
//    for(int i=0; i<5; i++){
//        cv::randomCrop(src, dst, cv::Size(200, 200));
//        printf("%d\t%d\n", dst.rows, dst.cols);
//        cv::imshow("lena.png", dst);
//        cv::waitKey(0);
//
//    }
//
//    cv::Resize resize = cv::Resize(cv::Size(200, 200));
//    resize.call(src, dst);
//    cv::imshow("lena.png", dst);
//    cv::waitKey(0);

    return 0;
}
