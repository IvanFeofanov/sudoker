#ifndef SU_ALGORITHM_H
#define SU_ALGORITHM_H

#include "cv.h"
#include "highgui.h"

#include "su_capture_cell.h"

typedef std::vector<SuCaptureCell*>  SUCells;
typedef std::vector<cv::Point>       SUPoints;

namespace su
{
    const cv::Size CELL_SIZE  = cv::Size(40, 40);
    const cv::Size FIELD_SIZE = cv::Size(CELL_SIZE.width*9,
                                         CELL_SIZE.height*9);

    const cv::Size RECT_OF_INTEREST = cv::Size(su::CELL_SIZE.width*0.5,
                                               su::CELL_SIZE.height*0.7);
    class PointsComOnX{
    public:
        bool operator() (cv::Point a, cv::Point b) { return a.x > b.x; }
    };

    class PoinstComOnY{
    public:
        bool operator() (cv::Point a, cv::Point b) { return a.y < b.y; }
    };
}

#endif // SU_ALGORITHM_H
