#ifndef SU_AM_RECOGNIZER_H
#define SU_AM_RECOGNIZER_H

#include <string>
#include <vector>

#include <algorithm>

#include "su_abstract_recognizer.h"

class SuAMRecognizer : public SuAbstractRecognizer
{
public:
    SuAMRecognizer();
    SuAMRecognizer(std::vector< std::vector<std::string> >& files);
    SuAMRecognizer(std::string trainData);
    ~SuAMRecognizer();

    int  recognize(cv::Mat &image);
    void recognize(SUCells &cells);


    int  train    (std::vector< std::vector<std::string> >& files);
    int  write    (std::string path);
    int  writeToHeader(std::string path);
    int  read     (std::string path);
    int  readFromArray();

private:
    void init();
    cv::Point getNumberCenter(cv::Mat& imageCell);

    std::vector<cv::Mat> masks_;
    std::vector<cv::Mat> weights_;

    int nClasses_;
    int imageArea_;

};

class Cluster
{
public:
    Cluster();
    Cluster(int classIdx, double charge);

    bool operator() (const Cluster& a, const Cluster& b);

    int    classIdx_;
    double charge_;

};

#endif // SU_AM_RECOGNIZER_H
