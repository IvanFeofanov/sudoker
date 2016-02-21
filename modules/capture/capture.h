#ifndef CAPTURE_H
#define CAPTURE_H

#include <iostream>
#include <math.h>
#include <vector>
#include <time.h>

#include <cv.h>
#include <highgui.h>

//#define SIMPLE_METHOD_OF_SLIKING

class Capture
{
public:
    Capture();
    Capture(cv::VideoCapture* capture);
    Capture(cv::VideoCapture* capture,
            float             maxDeviationBetweenFrames,
            int               minCountFrame);
   
    int                     searchField     ();
    std::vector<cv::Point>* getRect         ();
    cv::Mat*                getFieldImage   ();
    cv::Mat*                getFrame        ();
    bool                    getCells        (std::vector<cv::Mat*>& cells);
    
    static bool cutFieldFromImage(  cv::Mat&                image,
                                    cv::Mat&                field,
                                    std::vector<cv::Point>& rect );

    static bool cutFieldFromImage(  cv::Mat&                image,
                                    std::vector<cv::Mat*>&  cells,
                                    std::vector<cv::Point>& rect );

    //size images
    static const cv::Size  field_size;
    static const cv::Size  cell_size;

    enum 
    {
        FIELD_NOT_FOUND = 0,
        FIELD_FOUND     = 1,
        FIELD_CAPTURED  = 2
    };

private:
    //the accuracy of the measurements 
    static const float  min_len_side_field_per_; 
    static const float  approx_accuracy_per_;
    static const float  max_diff_len_side_;
    static const float  max_deviation_angle_;
    
    //check for motion
    bool    is_check_for_motion_;
    int     count_frame_;
    int     min_count_frame_;
    float   max_deviation_between_frames_;
    
    //capture
    cv::VideoCapture*         capture_;
    std::vector<cv::Point>    rect_;
    std::vector<cv::Point>    prevRect_;
    cv::Mat                   fieldImage_;
    cv::Mat                   image_;
    cv::Mat                   gray_image_;
    

    static float   calcLenLine_       ( cv::Point b, cv::Point e);
    static double  calcAngleLine_     ( cv::Point b, cv::Point e);

    static bool    cutMaxRect_        ( cv::Mat&                image,
                                        cv::Mat&                field,
                                        std::vector<cv::Point>& rect);

    static void    fieldToCutOnCells_ ( cv::Mat&                field,
                                        std::vector<cv::Mat*>&  cells);

};
    

#endif
