#include "su_capture_cell.h"

SuCaptureCell::SuCaptureCell()
{
    value_ = 0;
    point_ = cv::Point(0, 0);
}

SuCaptureCell::SuCaptureCell(cv::Point point, cv::Mat image)
{
    point_ = point;
    image_ = image;
}

void SuCaptureCell::setValue(int value)
{
    value_ = value;
}

int SuCaptureCell::value()
{
    return value_;
}

cv::Mat* SuCaptureCell::image()
{
    return &image_;
}

cv::Point SuCaptureCell::point()
{
    return point_;
}
