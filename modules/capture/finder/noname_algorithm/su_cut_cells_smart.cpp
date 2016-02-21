#include "su_cut_cells_smart.h"

SuCutCellsSmart::SuCutCellsSmart()
{
}

void SuCutCellsSmart::cut(cv::Mat &imageField, SUCells &cells)
{
    cv::Rect roi      (0, 0, su::CELL_SIZE.width, su::CELL_SIZE.height);
    cv::Mat  bin_img  (su::FIELD_SIZE, CV_8UC1);


    int      border    =  10;
    cv::Mat  field_img(imageField.rows + border * 2, imageField.cols + border * 2, imageField.depth());
    cv::copyMakeBorder( imageField, field_img, border,
                        border, border, border, cv::BORDER_CONSTANT, cv::Scalar(0));

    cv::Size min_rect_size(   su::CELL_SIZE.width * 0.2, su::CELL_SIZE.height * 0.33);
    cv::Size max_rect_size  = su::CELL_SIZE;
    double   max_val        = 255;
    int      block_size     = 7;
    int      c              = 4;

    cv::adaptiveThreshold(  imageField, bin_img, max_val,
                            CV_ADAPTIVE_THRESH_MEAN_C,
                            CV_THRESH_BINARY_INV, block_size, c);

    cv::dilate(bin_img, bin_img, cv::Mat());
    cv::erode(bin_img, bin_img, cv::Mat());

    //find numbers
//    int len_mat = su::FIELD_SIZE.width * su::FIELD_SIZE.height;

    for(int x = 0; x < su::FIELD_SIZE.width; x++)
    {
        for(int y = 0; y < su::FIELD_SIZE.height; y++)
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
                //cv::rectangle(bin_img, rect, cv::Scalar(255)); only for debug
                roi.x = border+(rect.x+rect.width/2)-su::CELL_SIZE.width/2;
                roi.y = border+(rect.y+rect.height/2)-su::CELL_SIZE.height/2;

                if( roi.x < 0 || roi.x >= field_img.rows    ||
                    roi.y < 0 || roi.y >= field_img.cols    ||
                    roi.x + roi.width  >= field_img.rows    ||
                    roi.y + roi.height >= field_img.cols    )
                {
                    continue;
                }

                cv::Point numCtr    (rect.x+rect.width/2,
                                     rect.y+rect.height/2);
                cv::Point coordinate(numCtr.x / (su::FIELD_SIZE.width / 9),
                                     numCtr.y / (su::FIELD_SIZE.height/ 9));

                cv::Mat cell_img = cv::Mat(su::CELL_SIZE, CV_8UC1);
                cell_img = field_img(roi);
                cells.push_back(new SuCaptureCell(coordinate, cell_img));
            }
        }
    }

}
