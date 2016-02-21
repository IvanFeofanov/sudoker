#ifndef SU_CUT_CELLS_SMART_H
#define SU_CUT_CELLS_SMART_H

#include "su_abstract_cut_cells.h"

class SuCutCellsSmart : public SuAbstractCutCells
{
public:
    SuCutCellsSmart();

    void cut(cv::Mat &imageField, SUCells &cells);

private:
};

#endif // SU_CUT_CELLS_SMART_H
