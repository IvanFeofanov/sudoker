#ifndef SU_CAPTURE_CELL_H
#define SU_CAPTURE_CELL_H

#include <cv.h>

class SuCaptureCell
{
public:
    SuCaptureCell();
    SuCaptureCell(cv::Point point, cv::Mat image);

    void      setValue   (int value);
    int       value      ();
    cv::Mat*  image      ();
    cv::Point point      ();

private:
    cv::Mat   image_;
    cv::Point point_;
    int       value_;

};

#endif // SU_CAPTURE_CELL_H
