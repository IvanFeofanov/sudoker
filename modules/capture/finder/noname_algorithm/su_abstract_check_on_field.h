#ifndef SU_ABSTRACT_CHECK_ON_FIELD_H
#define SU_ABSTRACT_CHECK_ON_FIELD_H

#include <iostream>

#include "su_algorithm.h"

class SuAbstractCheckOnField
{
public:
    SuAbstractCheckOnField();

    virtual bool check(cv::Mat& imageField);
};

#endif // SU_ABSTRACT_CHECK_ON_FIELD_H
