#ifndef OPENCV_AUG_TRANSFORMS_HPP
#define OPENCV_AUG_TRANSFORMS_HPP

#include <opencv2/core.hpp>
#include <vector>

namespace cv{

    CV_EXPORTS_W void randomCrop(InputArray src, OutputArray dst, const Size& sz, const Vec4i& padding=Vec4i(), bool pad_if_need=false, int fill=0, int padding_mode=BORDER_CONSTANT);

    CV_EXPORTS_W void randomFlip(InputArray src, OutputArray dst, int flipCode=0, double p=0.5);

    /*
     * Base class for all augmentation classes
     */
    CV_EXPORTS_W class Transform{
    public:
        CV_WRAP virtual void call(InputArray src, OutputArray dst) const = 0;
        CV_WRAP virtual ~Transform() = default;
    };

    class CV_EXPORTS_W RandomCrop: public Transform{
    public:
        CV_WRAP explicit RandomCrop(const Size& sz, const Vec4i& padding=Vec4i(), bool pad_if_need=false, int fill=0, int padding_mode=BORDER_CONSTANT);
        CV_WRAP ~RandomCrop() override = default;
        CV_WRAP void call(InputArray src, OutputArray dst) const override;

        Size sz;
        Vec4i padding;
        bool pad_if_need;
        int fill;
        int padding_mode;
    };

    class CV_EXPORTS_W RandomFlip: public Transform{
    public:
        CV_WRAP explicit RandomFlip(int flipCode=0, double p=0.5);
        CV_WRAP ~RandomFlip() override = default;
        CV_WRAP void call(InputArray src, OutputArray dst) const override;

        int flipCode;
        double p;
    };

    class CV_EXPORTS_W Compose{
    public:
        CV_WRAP explicit Compose(std::vector<Transform*> transforms);
        CV_WRAP void call(InputArray src, OutputArray dst) const;

        std::vector<Transform*> transforms;
    };

}

#endif
