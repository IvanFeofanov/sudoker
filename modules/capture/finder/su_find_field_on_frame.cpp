#include <cassert>

#include "su_find_field_on_frame.h"

SuFindFieldOnFrame::SuFindFieldOnFrame(SuAbstractFindField* finder,
                                       bool checkingOnMovement = true) :
    SuAbstractFindField()
{
    finder_ = finder;

    isCheckOnMotion_ = checkingOnMovement;

    //default parameters
    min_time_between_frame_ = 1000 * 100;
    max_diff_between_frame_ = DEFAULT_SENSITIVITY_TO_MOVEMENT;
}


SuFindFieldOnFrame::~SuFindFieldOnFrame()
{
    delete finder_;
}

int SuFindFieldOnFrame::find(cv::Mat               &image,
                             std::vector<SUCells>  &cells,
                             std::vector<SUPoints> &nodelPoints)
{
    assert(!image.empty());

    static cv::Mat prev_frame;

    //find
    int stat = finder_->find(image, cells, nodelPoints);

    if(stat == SuAbstractFindField::NOT_FOUND)
    {
        return NOT_FOUND;
    }

    if(isCheckOnMotion_)
    {
        if(prev_frame.empty())
        {
            prev_frame      = image;
            time_prev_frame_ = clock();
            return MOVING;
        }

        if((clock() - time_prev_frame_) < min_time_between_frame_)
        {
            return MOVING;
        }

        cv::Mat diff(image.size(), CV_8UC1);
        cv::absdiff (image, prev_frame, diff);

        long integral = 0;

        //integration
        for(int x = 0; x < diff.cols; x++)
        {
            for(int y = 0; y < diff.rows; y++)
            {
                integral += diff.at<uchar>(y, x);
            }
        }

        double diff_per = (double)(integral) / (double)(diff.cols*diff.rows) / 255.0;

//        std::cout << "diff " << diff_per << std::endl;

        if(diff_per > max_diff_between_frame_) //moving
        {
            image.copyTo(prev_frame);
            time_prev_frame_ = clock();

            return MOVING;
        }
        else    //not moving
        {
            image.copyTo(prev_frame);
            return stat;
        }

    }

    return stat;
}

void SuFindFieldOnFrame::setCheckOnMovement(bool stat)
{
    isCheckOnMotion_ = stat;
}

void SuFindFieldOnFrame::setSensitivityToMovement(double diff)
{
    max_diff_between_frame_ = (1.0 - diff) / 100.0;
}

bool SuFindFieldOnFrame::isCheckingOnMovement()
{
    return isCheckOnMotion_;
}

double SuFindFieldOnFrame::sensitivityToMovement()
{
    return 1.0f - max_diff_between_frame_ * 100;
}
