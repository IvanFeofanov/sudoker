#ifndef SU_ABSTRACT_FIND_RECT_H
#define SU_ABSTRACT_FIND_RECT_H

#include <cmath>
#include <vector>

#include <cv.h>
#include <highgui.h>

#include "su_algorithm.h"

class SuAbstractFindRect
{
public:
    SuAbstractFindRect();

    virtual int find(cv::Mat&               image,
                     std::vector<SUPoints>& points) = 0;

};

#endif // SU_ABSTRACT_FIND_RECT_H
