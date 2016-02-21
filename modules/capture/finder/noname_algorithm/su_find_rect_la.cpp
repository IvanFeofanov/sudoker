#include "su_find_rect_la.h"

SuFindRectLA::SuFindRectLA()
{
    max_deviation_angle_    = 0.1;
    max_diff_len_side_      = 0.1;
    approx_accuracy_per_    = 0.04;
    min_len_side_field_per_ = 0.5;
}

int SuFindRectLA::find(cv::Mat               &image,
                       std::vector<SUPoints> &points)
{
    cv::Size srcSize      = image.size();
    int      minLenSide   = std::min(srcSize.width, srcSize.height)*min_len_side_field_per_;
    float    approxEps    = std::min(srcSize.width, srcSize.height)*approx_accuracy_per_;

    std::vector< std::vector<cv::Point> > contours;
    std::vector< cv::Vec4i >              hierarchyContour;
    cv::Mat                               contour;
    int                                   nContours=0;

    SUPoints                              lines;
    cv::Mat                               linesMat;

    // 1. filtering image
    cv::Mat       filteredImg(srcSize, 8, 1);
    cv::Mat       pyr(cv::Size(srcSize.width/2, srcSize.height/2), 8, 1);

    cv::pyrDown(image, pyr);
    cv::pyrUp  (pyr, filteredImg);
    pyr.release();

    // 2. search rect on image

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
        for(int c = 0; c < 4; c++)
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

        //this contour is rect
        points.push_back(lines);
    }

    removeDuplicates(points);

    return true;
}

float SuFindRectLA::calcLenLine_(cv::Point b, cv::Point e)
{
    int delta_x = b.x - e.x;
    int delta_y = b.y - e.y;

    return sqrt(delta_x*delta_x + delta_y*delta_y);
}

void SuFindRectLA::removeDuplicates(std::vector<SUPoints> &points)
{
    for(int i = 0; i < points.size(); i++)
    {
        SUPoints* nodes1  = &points.at(i);
        cv::Point center1 = cv::Point((nodes1->at(0).x + nodes1->at(2).x) / 2,
                                      (nodes1->at(0).y + nodes1->at(2).y) / 2);
        for(int j = 0; j < points.size(); j++)
        {
            if(i == j)
                continue;

            SUPoints* nodes2 = &points.at(j);
            cv::Point center2 = cv::Point((nodes2->at(0).x + nodes2->at(2).x) / 2,
                                          (nodes2->at(0).x + nodes2->at(2).y) / 2);

            double dst = sqrt(pow(center1.x-center2.x, 2) +
                              pow(center1.y-center2.y, 2) );

            double min_dst = sqrt(pow(nodes1->at(0).x - nodes1->at(2).x, 2) +
                                  pow(nodes1->at(0).y - nodes2->at(2).y, 2)) / 1.5;

            if(dst < min_dst)
            {
                points.erase(points.begin()+j);
                if(i < j)
                    j--;
            }
        }
    }
}
