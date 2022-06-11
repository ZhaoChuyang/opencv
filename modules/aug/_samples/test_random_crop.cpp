#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/aug.hpp>


int main(int argv, char **argc) {
    const char* filename = argc[1];
    cv::Mat src = cv::imread(filename);
    cv::imshow("lena.png", src);
    cv::waitKey(0);

//    cv::Mat dst;
//    cv::randomCrop(src, dst, cv::Size(100, 100));

    return 0;
}
