#include <cassert>

#include "su_noname_finder.h"


SuNonameFinder::SuNonameFinder(SuAbstractFindRect*     finder,
                               SuAbstractCutCells*     cutter,
                               SuAbstractCheckOnField* validator)
{
    assert(finder != 0 && cutter != 0 && validator != 0);

    finder_     = finder;
    cutter_     = cutter;
    isRectField_= validator;
}

int SuNonameFinder::find(cv::Mat&               image,
                         std::vector<SUCells>&  cells,
                         std::vector<SUPoints>& nodelPoints)
{
    assert(!image.empty());
    assert(image.channels() == 1);

    int isOk = 1;

    //clear
    cells.clear();
    nodelPoints.clear();

    cv::Mat     imageField(su::FIELD_SIZE, CV_8UC1);
    SUCells     vector_of_cells;

    //1. find rects
    isOk = finder_->find(image, nodelPoints);
    if(!isOk)
    {
        return ERROR;
    }

    int nFields = nodelPoints.size();

    //for each potential image of field
    for(int i = 0; i < nFields; i++)
    {
        vector_of_cells.clear();

        //2. recovery of prospect, cutting of field from image
        cut_field(image, nodelPoints.at(i), imageField);

        //3. check on field
        if(!isRectField_->check(imageField))
        {
            nodelPoints.erase(nodelPoints.begin()+i);
            continue;
        }

        //4. cut cells
        cutter_->cut    (imageField, vector_of_cells);

        cells.push_back (vector_of_cells);

    }

    if(cells.size() == 0)
    {
        return NOT_FOUND;
    }

    return CAPTURED;
}

SuNonameFinder::~SuNonameFinder()
{
    delete finder_;
    delete cutter_;
    delete isRectField_;
}

void SuNonameFinder::cut_field(cv::Mat&  image,
                               SUPoints& nodes,
                               cv::Mat&  imageField)
{
    SUPoints    points = nodes;
    cv::Point2f srcNode[4];
    cv::Point2f dstNode[4];
    cv::Mat     warp_mat(3, 3, CV_32FC1);

    su::PoinstComOnY pointsComOnY;
    su::PointsComOnX pointsComOnX;
    //dst nodes
    dstNode[0].x = 0;  //left top point
    dstNode[0].y = 0;

    dstNode[1].x = su::FIELD_SIZE.width-1; //right top point
    dstNode[1].y = 0;

    dstNode[2].x = 0;   //left bottom point
    dstNode[2].y = su::FIELD_SIZE.height-1;

    dstNode[3].x = su::FIELD_SIZE.width - 1; // right bottom point
    dstNode[3].y = su::FIELD_SIZE.height- 1;

    //copy src nodes
    //search top and bottom points
    std::sort(points.begin(), points.end(), pointsComOnY);
    //search right and left top points
    std::sort(points.begin(), points.begin()+2, pointsComOnX);
    //serch right and left bottom points
    std::sort(points.begin()+2, points.end(), pointsComOnX);

    srcNode[0] = points.at(1);  //left top point
    srcNode[1] = points.at(0);  //right top point
    srcNode[2] = points.at(3);  //left bottom point
    srcNode[3] = points.at(2);  //right bottom point

    //transform
    warp_mat = cv::getPerspectiveTransform(srcNode, dstNode);
    cv::warpPerspective(image, imageField, warp_mat, su::FIELD_SIZE);
}
