#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <math.h>
#include <vector>

#include <cv.h>
#include <highgui.h>

class Parser
{
public:
    Parser();
    ~Parser();

    void cut(   cv::Mat&                img,
                std::vector<cv::Mat*>&  cells,
                std::vector<cv::Point>& rect);

private:
    //точность измерений
    float        min_len_side_field_per_; 
    float        approx_accuracy_per_;
    float        max_diff_len_side_;
    float        max_deviation_angle_;
    unsigned int max_lightness_cell_;

    //размеры изображений
    cv::Size    field_size_;
    cv::Size    cell_size_;
    
    float   calcLenLine_      (CvPoint b, CvPoint e);
    double  calcAngleLine_    (CvPoint b, CvPoint e);
};

#endif
