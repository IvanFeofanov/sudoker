#ifndef SU_FIND_RECT_LA_H
#define SU_FIND_RECT_LA_H

#include "su_algorithm.h"
#include "su_abstract_find_rect.h"

class SuFindRectLA : public SuAbstractFindRect
{
public:
    SuFindRectLA();

    int find(cv::Mat&               image,
             std::vector<SUPoints>& points);

//    void setMinLenSideFieldPer(float val);
//    void setApproxAccPer      (float val);
//    void setMaxDiffLenSide    (float val);
//    void setDevAngle          (float val);

private:
    float min_len_side_field_per_;
    float approx_accuracy_per_;
    float max_diff_len_side_;
    float max_deviation_angle_;

    float calcLenLine_( cv::Point b, cv::Point e);

    void  removeDuplicates(std::vector<SUPoints>& points);
};

#endif // SU_FIND_RECT_LA_H
