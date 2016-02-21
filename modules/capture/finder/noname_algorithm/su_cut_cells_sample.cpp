#include "su_cut_cells_sample.h"

SuCutCellsSample::SuCutCellsSample()
{
}

void SuCutCellsSample::cut(cv::Mat &imageField, SUCells &cells)
{
    cv::Rect roi(0, 0, su::CELL_SIZE.width, su::CELL_SIZE.height);

    for(int x = 0; x < 9; x++)
    {
        for(int y = 0; y < 9; y++)
        {
            roi.x = su::CELL_SIZE.width  * x;
            roi.y = su::CELL_SIZE.height * y;

            cv::Mat cell_img = cv::Mat(su::CELL_SIZE, CV_8UC1);
            cell_img = imageField(roi);
            cells.push_back(new SuCaptureCell(cv::Point(x, y),
                                              cell_img) );
        }
    }
}
