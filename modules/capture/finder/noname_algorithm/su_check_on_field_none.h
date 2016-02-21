#ifndef SU_CHECK_ON_FIELD_NONE_H
#define SU_CHECK_ON_FIELD_NONE_H

#include "su_abstract_check_on_field.h"

class SuCheckOnFieldNone : public SuAbstractCheckOnField
{
public:
    SuCheckOnFieldNone();

    bool check(cv::Mat &imageField);
};

#endif // SU_CHECK_ON_FIELD_NONE_H
