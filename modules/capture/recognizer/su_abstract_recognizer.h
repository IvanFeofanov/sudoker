#ifndef SU_ABSTRACT_RECOGNIZER_H
#define SU_ABSTRACT_RECOGNIZER_H

#include <string>

#include <cv.h>
#include <highgui.h>

#include "su_algorithm.h"

class SuAbstractRecognizer
{
public:
    SuAbstractRecognizer();

    virtual int  recognize(cv::Mat& image);
    virtual void recognize(SUCells cells);

private:

};

#endif // SU_ABSTRACT_RECOGNIZER_H
