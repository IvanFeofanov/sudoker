#ifndef SU_CUT_CELLS_SAMPLE_H
#define SU_CUT_CELLS_SAMPLE_H

#include "su_abstract_cut_cells.h"

class SuCutCellsSample : public SuAbstractCutCells
{
public:
    SuCutCellsSample();

    void cut(cv::Mat &imageField, SUCells &cells);

};

#endif // SU_CUT_CELLS_SAMPLE_H
