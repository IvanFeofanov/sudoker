#ifndef SU_FIND_FIELD_ON_FRAME_H
#define SU_FIND_FIELD_ON_FRAME_H

#include <ctime>

#include "su_abstract_find_field.h"


class SuFindFieldOnFrame : public SuAbstractFindField
{
public:
    explicit SuFindFieldOnFrame(SuAbstractFindField* finder,
                                bool checkingOnMovement);
    ~SuFindFieldOnFrame();

    int find(cv::Mat &image,
             std::vector<SUCells> &cells,
             std::vector<SUPoints> &nodelPoints);

    void   setCheckOnMovement       (bool stat);
    bool   isCheckingOnMovement     ();
    void   setSensitivityToMovement (double diff);
    double sensitivityToMovement    ();

    enum{
        MOVING    =  3,
    };



private:
    SuAbstractFindField* finder_;

    long   time_prev_frame_;

    bool   isCheckOnMotion_;
    long   min_time_between_frame_;
    double max_diff_between_frame_;

    static const float DEFAULT_SENSITIVITY_TO_MOVEMENT = 0.007;
};

#endif // SU_FIND_FIELD_ON_FRAME_H
