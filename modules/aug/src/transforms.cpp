#include "precomp.hpp"
#include <opencv2/highgui.hpp>

namespace cv{

    static void getRandomCropParams(int h, int w, int th, int tw, int* x, int* y);

    // NOTE: cv::randomCrop or randomCrop?
    void randomCrop(InputArray _src, OutputArray _dst, const Size& sz, const Vec4i& padding, bool pad_if_need, int fill, int padding_mode){
        // FIXME: whether the size of src should be (src.cols+left+right, src.rows+top+bottom)

        Mat src = _src.getMat();
        _dst.create(sz, src.type());
        Mat dst = _dst.getMat();

        if(padding != Vec4i()){
            copyMakeBorder(src, src, padding[0], padding[1], padding[2], padding[3], padding_mode, fill);
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
        Mat RoI(src, Rect(x, y, sz.width, sz.height));
        RoI.copyTo(dst);
    }

    static void getRandomCropParams(int h, int w, int th, int tw, int* x, int* y){
        if(h+1 < th || w+1 < tw){
            CV_Error( Error::StsBadSize, "The cropped size is larger than the image size" );
        }
        if(h == th && w == tw){
            (*x) = 0;
            (*y) = 0;
            return;
        }
//        time_t t;
//        srand((unsigned)time(&t));
//        (*x) = static_cast<int> (rand() / static_cast<float> (RAND_MAX) * (w-tw+1));
//        (*y) = static_cast<int> (rand()/ static_cast<float> (RAND_MAX) * (h-th+1));
        RNG rng = RNG(getTickCount());
        (*x) = rng.uniform(0, w-tw+1);
        (*y) = rng.uniform(0, h-th+1);

    }

    RandomCrop::RandomCrop(const Size& sz, const Vec4i& padding, bool pad_if_need, int fill, int padding_mode):
        sz (sz),
        padding (padding),
        pad_if_need (pad_if_need),
        fill (fill),
        padding_mode (padding_mode){};

    void RandomCrop::call(InputArray src, OutputArray dst) const{
        randomCrop(src, dst, sz, padding, pad_if_need, fill, padding_mode);
    }

    void randomFlip(InputArray _src, OutputArray _dst, int flipCode, double p){
        /*
         * flipCode:
         * 0 is vertical flip
         * 1 is horizontal flip
         * -1 is flip bott horizontally and vertically
         */
//        RNG& rng = theRNG();
        RNG rng = RNG(getTickCount());
        bool flag = rng.uniform(0., 1.) < p;
//        srand((unsigned)time(nullptr));
//        bool flag = static_cast<float> (rand() / RAND_MAX) < p;

        Mat src = _src.getMat();
        _dst.create(src.size(), src.type());
        Mat dst = _dst.getMat();
        if(!flag){
            src.copyTo(dst);
            return;
        }
        flip(src, dst, flipCode);
    }

    RandomFlip::RandomFlip(int flipCode, double p):
        flipCode(flipCode),
        p(p){};

    void RandomFlip::call(InputArray src, OutputArray dst) const{
        randomFlip(src, dst);
    }

    Compose::Compose(std::vector<Ptr<Transform> >& transforms):
        transforms(transforms){};

    void Compose::call(InputArray _src, OutputArray _dst) const{
        Mat src = _src.getMat();

        for(auto it = transforms.begin(); it != transforms.end(); ++it){
            (*it)->call(src, src);
        }
        src.copyTo(_dst);
    }

    Resize::Resize(const Size& sz, int interpolation):
        sz(sz),
        interpolation(interpolation){};

    void Resize::call(InputArray src, OutputArray dst) const{
        resize(src, dst, sz, 0, 0, interpolation);
    }

    // size: (width, height)
    void centerCrop(InputArray _src, OutputArray _dst, const Size& size) {
        Mat src = _src.getMat();
        Mat padded(src);
        // pad the input image if needed
        if (size.width > src.cols || size.height > src.rows) {
            int top = size.height - src.rows > 0 ? static_cast<int>((size.height - src.rows) / 2) : 0;
            int bottom = size.height - src.rows > 0 ? static_cast<int>((size.height - src.rows) / 2) : 0;
            int left = size.width - src.cols > 0 ? static_cast<int>((size.width - src.cols) / 2) : 0;
            int right = size.width - src.cols > 0 ? static_cast<int>((size.width - src.cols) / 2) : 0;

            // fill with value 0
            copyMakeBorder(src, padded, top, bottom, left, right, BORDER_CONSTANT, 0);
        }

        int x = static_cast<int>((padded.cols - size.width) / 2);
        int y = static_cast<int>((padded.rows - size.height) / 2);
        
        Mat cropped(padded, Rect(x, y, size.width, size.height));
        _dst.create(size, src.type());
        Mat dst = _dst.getMat();
        // Ensure the size of the cropped image is the same as the size of the dst
        CV_Assert(cropped.size() == dst.size() && cropped.type() == dst.type());
        cropped.copyTo(dst);
    }

    CenterCrop::CenterCrop(const Size& size) :
        size(size) {};

    void CenterCrop::call(InputArray src, OutputArray dst) const {
        centerCrop(src, dst, size);
    }
}
