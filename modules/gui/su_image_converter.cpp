#include "su_image_converter.h"

SuImageConverter::SuImageConverter()
{
}

int SuImageConverter::cvmat2qpixmap(const cv::Mat& mat,
                                    QPixmap& pixmap)
{
    cv::Mat dstMat = cv::Mat(mat.size(), CV_8UC4);

    switch(mat.channels())
    {
    case 3:
        cv::cvtColor(mat, dstMat, CV_BGR2RGBA);
        break;

    case 4:
        cv::cvtColor(mat, dstMat, CV_BGR2RGBA);
        break;

    case 1:
        cv::cvtColor(mat, dstMat, CV_GRAY2RGBA);
        break;

    default:
        return ERROR;
    }

    QImage image((const uchar*) dstMat.data,
                 dstMat.cols,
                 dstMat.rows,
                 dstMat.step,
                 QImage::Format_RGBA8888);

    image.bits();

    pixmap.convertFromImage(image);

    return OK;
}
