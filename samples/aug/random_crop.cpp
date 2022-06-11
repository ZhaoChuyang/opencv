#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/aug.hpp>
#include <iostream>


int main(int argv, char **argc) {
    const char* filename = argc[1];
    cv::Mat src = cv::imread(filename);
//    cv::imshow("lena.png", src);
//    cv::waitKey(0);

    for(int i=0; i<3; i++){
        cv::Mat dst;
        cv::randomCrop(src, dst, cv::Size(600, 200), cv::Vec4i(), true);
        cv::imshow("lena.png", src);
        std::cout << src.rows << " " << src.cols << std::endl;
        cv::waitKey(0);
    }


    return 0;
}
