#include "capture.h"

//the accuracy of the measurements
const  float Capture::min_len_side_field_per_ = 0.5;  //50% от размера изображения
const  float Capture::approx_accuracy_per_    = 0.04;  
const  float Capture::max_diff_len_side_      = 0.04; 
const  float Capture::max_deviation_angle_    = 0.04;

//size images
const cv::Size Capture::cell_size  = cv::Size(40, 40);
const cv::Size Capture::field_size = cv::Size( cell_size.width*9,
                                               cell_size.height*9);

Capture::Capture()
{
    capture_ = NULL;
}



Capture::Capture(cv::VideoCapture* capture)
{
    capture_    = capture;
    fieldImage_ = cv::Mat(field_size, 8, 1);
    
    *capture_ >> image_; 
    gray_image_ = cv::Mat::zeros(image_.size(), CV_8UC1);
    
    is_check_for_motion_             = false;
}



Capture::Capture(   cv::VideoCapture* capture,
                    float             maxDeviationBetweenFrames,
                    int               minCountFrame )
{
    capture_    = capture;
    fieldImage_ = cv::Mat(field_size, 8, 1);
    
    *capture_ >> image_; 
    gray_image_ = cv::Mat::zeros(image_.size(), CV_8UC1);
    
    is_check_for_motion_             = true;
    max_deviation_between_frames_    = maxDeviationBetweenFrames;
    min_count_frame_                 = minCountFrame;
    count_frame_                     = minCountFrame;
}

bool Capture::cutFieldFromImage( cv::Mat&                image,
                                 cv::Mat&                field,
                                 std::vector<cv::Point>& rect )
{
    bool isOk = true;

    isOk = cutMaxRect_(image, field, rect);
    
    return isOk;
}



bool Capture::cutFieldFromImage( cv::Mat&                image,
                                 std::vector<cv::Mat*>&  cells,
                                 std::vector<cv::Point>& rect )
{
    bool isOk = true;

    cv::Mat fieldImage     (field_size, 8, 1);

    isOk = cutMaxRect_     (image, fieldImage, rect);

    if(isOk){
        fieldToCutOnCells_ (fieldImage, cells);
    }

    return isOk;
}



int Capture::searchField()
{
    //1. get image
    cv::Mat src;
    *capture_ >> image_;
    cv::cvtColor(image_, gray_image_, CV_RGB2GRAY);

    //2. search rect
    bool isOk = true;
    
    isOk = cutMaxRect_(gray_image_, fieldImage_, rect_);

    if(!isOk)
    {
        count_frame_ = 0;
        return FIELD_NOT_FOUND;
    }
    
    if(!is_check_for_motion_)
    {
        return FIELD_CAPTURED;
    }
    
    //3. check for motion
    if(count_frame_ == 0)
    {
        prevRect_  = rect_;
    }

    count_frame_++;

    if(count_frame_ >= min_count_frame_)
    {
        if(!prevRect_.empty())
        {
           float pos_deviation = 0.00f;     
           float sum_deviation = 0.00f;
           
            for(int i=0; i<4; i++)
            {
                pos_deviation += pow(prevRect_.at(i).x - rect_.at(i).x, 2); 
                pos_deviation += pow(prevRect_.at(i).y - rect_.at(i).y, 2);
                sum_deviation += sqrt(pos_deviation);
                pos_deviation =  0.0f;
            } 

            sum_deviation = sqrt(sum_deviation);
            
            count_frame_=0;

            if(sum_deviation < max_deviation_between_frames_)
            {
                return FIELD_CAPTURED;
            }
        }
    }

    return FIELD_FOUND;
}



std::vector<cv::Point>* Capture::getRect()
{
    return &rect_;
}



cv::Mat* Capture::getFieldImage()
{
    if(fieldImage_.empty())
    {
        return NULL;
    }

    return &fieldImage_;
}

 

cv::Mat* Capture::getFrame()
{
    return &image_;
}



bool Capture::getCells(std::vector<cv::Mat*>& cells)
{
    if(fieldImage_.empty())
    {
        return false;
    }

    cells.clear();

    fieldToCutOnCells_(fieldImage_, cells);

    return true;
}



bool Capture::cutMaxRect_( cv::Mat&                image,
                           cv::Mat&                field,
                           std::vector<cv::Point>& rect )
{
    cv::Size srcSize      = image.size();
    int      minLenSide   = std::min(srcSize.width, srcSize.height)*min_len_side_field_per_;
    float    approxEps    = std::min(srcSize.width, srcSize.height)*approx_accuracy_per_;
    
    std::vector< std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i>                hierarchyContour;
    cv::Mat                               contour;
    int                                   nContours=0;

    std::vector<cv::Point>                lines;
    cv::Mat                               linesMat;

    // 1. filtering image
    cv::Mat       filteredImg(srcSize, 8, 1);
    cv::Mat       pyr(cv::Size(srcSize.width/2, srcSize.height/2), 8, 1);

    cv::pyrDown(image, pyr);
    cv::pyrUp  (pyr, filteredImg);
    pyr.release();
    
    // 2. search field on image
    bool isFound = false;

    //binarization
    cv::Mat     binImg (srcSize, 8, 1);
    cv::Canny          (filteredImg, binImg, 0, 100, 5);
    cv::dilate         (binImg, binImg, cv::Mat());
   

    //find contours
    cv::findContours( binImg,
                      contours,
                      hierarchyContour,
                      CV_RETR_LIST,
                      CV_CHAIN_APPROX_SIMPLE );

    nContours   = contours.size();

    for(int i=0; i<nContours; i++)
    {
        //search line(approx contour)
        contour = cv::Mat(contours.at(i));
        lines.clear();
        cv::approxPolyDP (contour,
                          lines,
                          approxEps,
                          true);
        
        linesMat = cv::Mat(lines); 
                
        //check whether the contour is a square
        int        count_node    = lines.size();
        bool       isOk          = true;
        cv::Point  nodes[5];

        //
        if(count_node != 4)
            continue;
        
        //copy nodes point from vector to arr
        for(int c = 0; c<4; c++)
        {
            nodes[c] = linesMat.at<cv::Point>(0, c);
            
            if(nodes[c].x <= 0 || nodes[c].x >= srcSize.width ||
               nodes[c].y <= 0 || nodes[c].y >= srcSize.height   )
            {
                isOk = false;
            }
        }
        nodes[4] = linesMat.at<cv::Point>(0, 0);
        
        if(!isOk)
            continue;


        if(!cv::isContourConvex(lines))
            continue;

        int lenFirstLine = calcLenLine_(nodes[0], nodes[1]);

        //if side is very short 
        if(lenFirstLine < minLenSide)
            continue;

        //if sides are different on lenght
        for(int i=1; i<count_node; i++)
        {
            if( (float)abs(lenFirstLine - calcLenLine_(nodes[i], nodes[i+1])) /
                (float)(lenFirstLine) >  max_diff_len_side_ )
            {
                isOk    = false;
                break;
            }
        }

        if(!isOk)
            continue;

        //compare the diagonal line
        if( fabs(calcLenLine_(nodes[0], nodes[2]) - calcLenLine_(nodes[1], nodes[3])) /
            calcLenLine_(nodes[0], nodes[2]) > max_deviation_angle_ )
        {
            continue;
        }

        //this contour is square
        
        //rotation image
        double         angle            = 180 / M_PI * calcAngleLine_(nodes[0], nodes[1]);
        double         angleRotation    = -1*(((int)(angle)/90 * 90 + 90)-angle);
        cv::Point2f    centerRot          ((nodes[0].x + nodes[2].x) / 2,
                                           (nodes[0].y + nodes[2].y) / 2 );
        double         scale            = (float)field_size.width / (float)lenFirstLine;
        cv::Mat        rotMat             (2, 3, CV_32FC1);
        cv::Mat        tmpMat             (srcSize, image.type());
        rotMat                          = cv::getRotationMatrix2D(centerRot,
                                                                  angleRotation,
                                                                  scale);
        cv::warpAffine(image, tmpMat, rotMat, srcSize);

        //cut field from image
        cv::Rect roi = cv::Rect(centerRot.x-field_size.width/2,
                                centerRot.y-field_size.height/2,
                                field_size.width,
                                field_size.height );

        if( roi.x <= 0 || roi.x + roi.width  >= srcSize.width ||
            roi.y <= 0 || roi.y + roi.height >= srcSize.height )
        {
            continue;
        }

        field   = tmpMat(roi);
        
        //copy nodes
        rect = lines;
        
        isFound = true;
        
        /*debug*/
        //std::vector<cv::Mat*> cells;
        //fieldToCutOnCells_(field, cells);

        break;
    }

    return isFound;
} 



void Capture::fieldToCutOnCells_(cv::Mat&               field,
                                 std::vector<cv::Mat*>& cells)
{
#ifndef SIMPLE_METHOD_OF_SLIKING

    cv::Rect roi      (0, 0, cell_size.width, cell_size.height);
    cv::Mat  bin_img  (field_size, CV_8UC1);
    
    
    int      border    =  10;
    cv::Mat  field_img(field.rows + border * 2, field.cols + border * 2, field.depth()); 
    cv::copyMakeBorder( field, field_img, border,
                        border, border, border, cv::BORDER_CONSTANT, cv::Scalar(0));
    
    cv::Size min_rect_size(   cell_size.width * 0.2, cell_size.height * 0.33);
    cv::Size max_rect_size  = cell_size;
    double   max_val        = 255;
    int      block_size     = 7;
    int      c              = 4;

    cv::adaptiveThreshold(  field, bin_img, max_val,
                            CV_ADAPTIVE_THRESH_MEAN_C,
                            CV_THRESH_BINARY_INV, block_size, c);
    
    cv::dilate(bin_img, bin_img, cv::Mat());
    cv::erode(bin_img, bin_img, cv::Mat());
    
    //find numbers
    int len_mat = field_size.width*field_size.height;
    
    for(int x=0; x<field_size.width; x++)
    {
        for(int y=0; y<field_size.height; y++)
        {
            if(bin_img.at<uchar>(y, x) != 255)
            {
                continue;
            }

            cv::Rect rect;
            cv::floodFill(bin_img, cv::Point(x, y), cv::Scalar(200), &rect);

            if( rect.width  >= min_rect_size.width  &&
                rect.height >= min_rect_size.height &&
                rect.width  <= max_rect_size.width  &&
                rect.height <= max_rect_size.height   )
            {
                //cv::rectangle(bin_img, rect, cv::Scalar(255));
                roi.x = border+(rect.x+rect.width/2)-cell_size.width/2;
                roi.y = border+(rect.y+rect.height/2)-cell_size.height/2;
                
                if( roi.x < 0 || roi.x >= field_img.rows    ||
                    roi.y < 0 || roi.y >= field_img.cols    ||
                    roi.x + roi.width  >= field_img.rows    ||
                    roi.y + roi.height >= field_img.cols    )
                {
                    continue;
                }

                cv::Mat* cell_img = new cv::Mat(cell_size, CV_8UC1);
                *cell_img = field_img(roi);
                cells.push_back(cell_img);
            }
        }
    }

    /*debug*/
    //cv::namedWindow("bin", 0);
    //cv::imshow("bin", field_img);
    //cv::waitKey(0);

#else
    
    cv::Rect roi    (0, 0, cell_size.width, cell_size.height);

    for(int x = 0; x < 9; x++)
    {
        for(int y = 0; y < 9; y++)
        {
            roi.x = cell_size.width  * x;
            roi.y = cell_size.height * y;

            cv::Mat* cell_img = new cv::Mat(cell_size, CV_8UC1);
            *cell_img = field(roi);
            cells.push_back(cell_img);
        }
    }
    
#endif
}


float Capture::calcLenLine_(cv::Point b, cv::Point e)
{
    int delta_x = b.x - e.x;
    int delta_y = b.y - e.y;

    return sqrt(delta_x*delta_x + delta_y*delta_y);
} 



double Capture::calcAngleLine_(cv::Point b, cv::Point e)
{
    float delta_x = b.x - e.x;
    float delta_y = b.y - e.y;
    
    double alpha;
    double tanAlpha;

    if(delta_x == 0.0f){
        alpha = delta_y > 0 ? M_PI/2 : M_PI/2+M_PI;
        return alpha;
    }

    tanAlpha = delta_y / delta_x;
    
    if(tanAlpha == 0.0f){
        if(delta_x < 0){
            alpha = M_PI;
        }else{
            alpha = 0.0f;
        }
        return alpha;
    }

    if(delta_y > 0){
        alpha = atan(tanAlpha);
    }else{
        alpha = M_PI + atan(tanAlpha);
    }

    return alpha;
} 
