#ifndef SU_IMAGE_CONVERTER_H
#define SU_IMAGE_CONVERTER_H

#include <cv.h>
#include <QPixmap>

class SuImageConverter
{
public:
    SuImageConverter();

    static int cvmat2qpixmap(const cv::Mat &mat,
                             QPixmap& pixmap);

    enum{
        ERROR = 0,
        OK    = 1
    };
};

#endif // SU_IMAGE_CONVERTER_H
