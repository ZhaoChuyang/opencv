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

    cv::RandomCrop randomCrop(cv::Size(200, 200));
    cv::RandomFlip randomFlip;
    std::vector<cv::Transform*> all_transforms = {&randomCrop, &randomFlip};
    cv::Compose transforms(all_transforms);

    for(int i=0; i<5; i++){
        transforms.call(src, dst);
        cv::imshow("lena.png", dst);
        cv::waitKey(0);
    }


    return 0;
}
