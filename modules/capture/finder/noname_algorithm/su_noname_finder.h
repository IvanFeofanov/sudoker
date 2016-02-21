#ifndef SU_NONAME_FINDER_H
#define SU_NONAME_FINDER_H

#include <iostream>
#include <vector>
#include <algorithm>

#include <cv.h>
#include <highgui.h>

#include "su_abstract_find_rect.h"
#include "su_abstract_cut_cells.h"
#include "su_abstract_check_on_field.h"

#include "su_find_rect_la.h"
#include "su_check_on_field_none.h"
#include "su_cut_cells_smart.h"

#include "finder/su_abstract_find_field.h"


class SuNonameFinder : public SuAbstractFindField
{
public:
    explicit SuNonameFinder(SuAbstractFindRect*     finder    = new SuFindRectLA(),
                            SuAbstractCutCells*     cutter    = new SuCutCellsSmart(),
                            SuAbstractCheckOnField* validator = new SuCheckOnFieldNone());
    ~SuNonameFinder();

    int find(cv::Mat&                image,
             std::vector<SUCells>&   cells,
             std::vector<SUPoints>&  nodelPoints);

protected:
    SuAbstractFindRect*     finder_;
    SuAbstractCutCells*     cutter_;
    SuAbstractCheckOnField* isRectField_;

private:
    virtual void cut_field(cv::Mat& image, SUPoints& nodes, cv::Mat& imageField);

};

#endif // SU_NONAME_FINDER_H
