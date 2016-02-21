#include "parser.h"



Parser::Parser()
{
    //точность измерений
    min_len_side_field_per_ = 0.5;  //50% от размера изображения
    approx_accuracy_per_    = 0.03;  
    max_diff_len_side_      = 0.03; 
    max_deviation_angle_    = 0.03;
    max_lightness_cell_     = 20;

    //размеры изображений
    cell_size_  = cv::Size(40, 40);
    field_size_ = cv::Size(cell_size_.width*9, cell_size_.height*9);


}


Parser::~Parser()
{
}



void Parser::cut(   cv::Mat&                img,
                    std::vector<cv::Mat*>&  cells,
                    std::vector<cv::Point>& rect )

{
    cv::Size srcSize(img.size());
    int      minLenSide      = std::min(srcSize.width, srcSize.height)*min_len_side_field_per_;
    float    approxEps       = std::min(srcSize.width, srcSize.height)*approx_accuracy_per_;
    
    std::vector< std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i>                hierarchyContour;
    cv::Mat                               contour;
    int                                   nContours=0;

    std::vector<cv::Point>                lines;
    cv::Mat                               linesMat;

    // 1. filtering image
    cv::Mat       filteredImg(srcSize, 8, 1);
    cv::Mat       pyr(cv::Size(srcSize.width/2, srcSize.height/2), 8, 1);

    cv::pyrDown(img, pyr);
    cv::pyrUp  (pyr, filteredImg);
    pyr.release();
    
    // 2. search field on image
    cv::Mat   fieldImage(field_size_, 8, 1);
    bool      isFound     = false;

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
            
            if(nodes[c].x < 0 || nodes[c].x >= srcSize.width ||
               nodes[c].y < 0 || nodes[c].y >= srcSize.height   )
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
        double         scale            = (float)field_size_.width / (float)lenFirstLine;
        cv::Mat        rotMat             (2, 3, CV_32FC1);
        cv::Mat        tmpMat             (srcSize, img.type());
        rotMat                          = cv::getRotationMatrix2D(centerRot,
                                                                  angleRotation,
                                                                  scale);
        cv::warpAffine(img, tmpMat, rotMat, srcSize);

        //cut field from image
        cv::Rect roi = cv::Rect(centerRot.x-field_size_.width/2,
                                centerRot.y-field_size_.height/2,
                                field_size_.width,
                                field_size_.height );
        fieldImage   = tmpMat(roi);
        
        //copy nodes
        rect = lines;

        isFound = true;

        break;
    }
     

    if(!isFound)
        return;
    
    // 3. cut cells
    cv::Rect roi(0, 0, cell_size_.width, cell_size_.height);

    for(int y=0; y<9; y++)
    {
        for(int x=0; x<9; x++)
        {
            roi.x   =   x*cell_size_.width;
            roi.y   =   y*cell_size_.height;

            cv::Mat* cellImage = new cv::Mat(fieldImage(roi));
            
            cells.push_back(cellImage);
        }
    }

}


float Parser::calcLenLine_(CvPoint b, CvPoint e)
{
    int delta_x = b.x - e.x;
    int delta_y = b.y - e.y;

    return sqrt(delta_x*delta_x + delta_y*delta_y);
}



double Parser::calcAngleLine_(CvPoint b, CvPoint e)
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
