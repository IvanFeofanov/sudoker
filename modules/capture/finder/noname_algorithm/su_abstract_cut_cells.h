#ifndef SU_ABSTRACT_CUT_CELLS_H
#define SU_ABSTRACT_CUT_CELLS_H

#include <iostream>
#include <vector>

#include "su_algorithm.h"

class SuAbstractCutCells
{
public:
    SuAbstractCutCells();

    virtual void cut(cv::Mat&   imageField,
                     SUCells&   cells);
};

#endif // SU_ABSTRACT_CUT_CELLS_H
