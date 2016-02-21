#ifndef SU_FIND_FIELD_ON_IMAGE_H
#define SU_FIND_FIELD_ON_IMAGE_H

#include "su_abstract_find_field.h"

#include "su_find_rect_la.h"
#include "su_check_on_field_none.h"
#include "su_cut_cells_sample.h"

class SuFindFieldOnImage : public SuAbstractFindField
{
public:
    explicit SuFindFieldOnImage();
};

#endif // SU_FIND_FIELD_ON_IMAGE_H
