#ifndef SU_ABSTRACT_FIND_FIELD_H
#define SU_ABSTRACT_FIND_FIELD_H

#include "su_algorithm.h"

class SuAbstractFindField
{
public:
    explicit SuAbstractFindField();

    virtual int find(cv::Mat&                image,
                     std::vector<SUCells>&   cells,
                     std::vector<SUPoints>&  nodelPoints) = 0;

    enum{
        CAPTURED  = 2,
        NOT_FOUND = 1,
        ERROR     = 0
    };

};

#endif // SU_ABSTRACT_FIND_FIELD_H
