#include <cassert>

#include "su_am_recognizer.h"

SuAMRecognizer::SuAMRecognizer()
{
    init();
}

SuAMRecognizer::SuAMRecognizer(std::vector<std::vector<std::string> > &files)
{
    init();
    train(files);
}

SuAMRecognizer::SuAMRecognizer(std::string trainData)
{
    init();
    int stat = 0;

    stat = read(trainData);
}

void SuAMRecognizer::init()
{
    nClasses_  = 9;
    imageArea_ = su::CELL_SIZE.area();
}

int SuAMRecognizer::train(std::vector<std::vector<std::string> > &files)
{
    unsigned int nImages = files.at(0).size();

    //1. calc mask

    //for each class
    for(int i = 0; i < nClasses_; i++)
    {
        cv::Mat sum = cv::Mat::zeros(su::CELL_SIZE, CV_32FC1);

        //for each images in the class
        for(unsigned int j = 0; j < nImages; j++)
        {
            //get file name
            std::string fileName = files.at(i).at(j);

            //load image
            cv::Mat image = cv::imread(fileName.c_str(), 0);
            assert(!image.empty());

            cv::Mat bin   = cv::Mat(su::CELL_SIZE, image.type());
            cv::adaptiveThreshold(image, bin, 255,
                                  CV_ADAPTIVE_THRESH_MEAN_C,
                                  CV_THRESH_BINARY, 7, 4);

            bin.convertTo(bin, CV_32FC1);

            //sum
            cv::add(sum, bin, sum);
        }

        sum = sum * 1.0000f / (double)(nImages);

        cv::Mat mask(su::RECT_OF_INTEREST, CV_32FC1);
        cv::Point center = getNumberCenter(sum);

        cv::Rect roi(   center.x-su::RECT_OF_INTEREST.width/2,
                        center.y-su::RECT_OF_INTEREST.height/2,
                        su::RECT_OF_INTEREST.width,
                        su::RECT_OF_INTEREST.height);

        mask = sum(roi);

        masks_.push_back(mask);

//        mask.convertTo(mask, CV_8UC1);
//        cv::namedWindow("cell", 0);
//        cv::imshow("cell", mask);
//        cv::waitKey(0);
    }

    //2. calc weight

    //calc middle image
    cv::Mat middle = cv::Mat::zeros(su::RECT_OF_INTEREST, CV_32FC1);

    for(int i = 0; i < nClasses_; i++)
    {
        cv::add(middle, masks_.at(i), middle);
    }

    middle = middle * 1.0000f / (double)(nClasses_);

//    middle.convertTo(middle, CV_8UC1);
//    cv::namedWindow("middle", 0);
//    cv::imshow("middle", middle);
//    cv::waitKey(0);

    //calc weight for each classes
    for(int i = 0; i < nClasses_; i++)
    {
        cv::Mat weight = cv::Mat::zeros(su::RECT_OF_INTEREST, CV_32FC1);

        cv::absdiff(middle, masks_.at(i), weight);
//        weight.convertTo(weight, CV_8UC1);
//        cv::namedWindow("cell", 0);
//        cv::imshow("cell", weight);
//        cv::waitKey(0);

        weight = weight * 1.0000f / (double)(255);

        weights_.push_back(weight);

   }
}

int SuAMRecognizer::recognize(cv::Mat &image)
{
    assert(!image.empty());

    cv::Mat src_image = cv::Mat(su::CELL_SIZE, image.type());
    cv::adaptiveThreshold(image, src_image, 255,
                          CV_ADAPTIVE_THRESH_MEAN_C,
                          CV_THRESH_BINARY, 7, 4);

//    cv::namedWindow("image", 0);
//    cv::imshow("image", src_image);
//    cv::waitKey(0);
//    cv::destroyAllWindows();

    src_image.convertTo(src_image, CV_32FC1);

    std::vector<Cluster> charges;

    for(int i = 0; i < nClasses_; i++)
    {
        int min_value = -1;

        //for each position
        for(int x = 0; x < su::CELL_SIZE.width - su::RECT_OF_INTEREST.width + 1; x += 1)
        {
            for(int y = 0; y < su::CELL_SIZE.height - su::RECT_OF_INTEREST.height + 1; y += 1)
            {
                cv::Rect roi(x, y, su::RECT_OF_INTEREST.width,
                             su::RECT_OF_INTEREST.height);

                cv::Mat diff;
                cv::absdiff(src_image(roi), masks_.at(i), diff);

                cv::Mat mult;
                cv::multiply(diff, weights_.at(i), mult);

//                diff.convertTo(diff, CV_8UC1);
//                cv::namedWindow("dd", 0);
//                cv::imshow("dd", masks_.at(i));
//                cv::waitKey(0);

                cv::Scalar_<double> charge = cv::sum(mult);

                if(min_value == -1){
                    min_value = charge[0];
                }else{
                    if(charge[0] < min_value){
                        min_value = charge[0];
                    }
                }
            }
        }

        charges.push_back(Cluster(i+1, min_value));

    }

    //find minimun charge
    std::sort(charges.begin(), charges.end(), Cluster());

    return charges.at(0).classIdx_;

}

void SuAMRecognizer::recognize(SUCells &cells)
{
    assert(cells.size() > 0);

    for(unsigned int i = 0; i < cells.size(); i++)
    {
        int classNum = recognize(*(cells.at(i)->image()));
        cells.at(i)->setValue(classNum);
    }
}

cv::Point SuAMRecognizer::getNumberCenter(cv::Mat &imageCell)
{
    cv::Mat bin_img = cv::Mat(su::CELL_SIZE, CV_32FC1);

    cv::threshold(imageCell, bin_img, 125.0, 255.0, CV_THRESH_BINARY_INV);

    cv::Size min_rect_size(   su::CELL_SIZE.width * 0.2, su::CELL_SIZE.height * 0.33);
    cv::Size max_rect_size  = su::CELL_SIZE;

//    cv::namedWindow("image", 0);
//    cv::imshow("image", bin_img);
//    cv::waitKey(0);

    for(int x = 0; x < su::CELL_SIZE.width; x++)
    {
        for(int y = 0; y < su::CELL_SIZE.height; y++)
        {
            if(bin_img.at<double>(y, x) < 255.0)
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
                cv::Point center(rect.x + rect.width / 2,
                                 rect.y + rect.height / 2);
                return center;
            }
        }
    }

    return cv::Point(-1, -1);
}

int SuAMRecognizer::write(std::string path)
{
    cv::FileStorage storage(path, cv::FileStorage::WRITE);
    assert(storage.isOpened());

    for(int i = 0; i < nClasses_; i++)
    {
        //save mask
        std::string maskName;
        maskName =  "mask_";
        maskName += (char)(i+1+'0');

        cv::write(storage, maskName, masks_.at(i));

        //saveWeight
        std::string weightName;
        weightName =  "weight_";
        weightName += (char)(i+1+'0');

        cv::write(storage, weightName, weights_.at(i));
    }

    storage.release();

    return 0;
}

int SuAMRecognizer::read(std::string path)
{
    cv::FileStorage storage(path, cv::FileStorage::READ);
    assert(storage.isOpened());

    //clear
    masks_.clear();
    weights_.clear();

    //read
    cv::FileNode node;

    for(int i = 0; i < nClasses_; i++)
    {
        cv::Mat mask;
        cv::Mat weight;

        //read mask
        std::string maskName;
        maskName =  "mask_";
        maskName += (char)(i+1+'0');

        node = storage[maskName];
        cv::read(node, mask);
        masks_.push_back(mask);

        //saveWeight
        std::string weightName;
        weightName =  "weight_";
        weightName += (char)(i+1+'0');

        node = storage[weightName];
        cv::read(node, weight);
        weights_.push_back(weight);
    }
    storage.release();

//    cv::namedWindow("he", 0);
//    for(int i = 0; i < nClasses_; i++)
//    {
//        cv::Mat image;
//        weights_.at(i).convertTo(image, CV_8UC1);
//        cv::imshow("he", image);
//        cv::waitKey(0);
//    }

    return 0;
}


//cluster
Cluster::Cluster()
{
    classIdx_ = 0;
    charge_   = 0.00f;
}

Cluster::Cluster(int classIdx, double charge)
{
    classIdx_ = classIdx;
    charge_   = charge;
}

bool Cluster::operator ()(const Cluster& a, const Cluster& b)
{
    return a.charge_ < b.charge_;
}

