#include "su_find_field_on_image.h"

SuFindFieldOnImage::SuFindFieldOnImage() :
    SuAbstractFindField( new SuFindRectLA,
                         new SuCutCellsSample,
                         new SuCheckOnFieldNone)
{
}
