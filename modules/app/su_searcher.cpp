#include "su_searcher.h"

#include <QDebug>

SuSearcher::SuSearcher(SuAppPreferences preferences, QObject *parent) :
    QObject(parent)
{
    source_     = IMAGE;

    finder_     = new SuFindFieldOnFrame(new SuNonameFinder,
                                     source_ == IMAGE ? true : false);
    recognizer_ = new SuAMRecognizer(/*preferences.pathToTrainData*/);

    cameraId_   = -1;

    updatePreferences(preferences);

}

SuSearcher::SuSearcher(){ }

SuSearcher::~SuSearcher()
{
    emit finished();

    qDebug() << "SuSearcher destruct";

    delete finder_;
    delete recognizer_;
}

void SuSearcher::updatePreferences(SuAppPreferences preferences)
{
    preferences_ = preferences;

    //finder
    finder_->setCheckOnMovement(preferences_.isCheckOnMotion);
    finder_->setSensitivityToMovement(preferences_.sensitivityToMovement);

    //recognizer
//    recognizer_->read(preferences_.pathToTrainData);
}

void SuSearcher::imageProcessing(QString pathToImage)
{
    source_ = IMAGE;

    pathToImage_ = pathToImage;

    finder_->setCheckOnMovement(false); //because static image
}

void SuSearcher::videoProcessing(int cameraId)
{
    source_ = VIDEO;

    finder_->setCheckOnMovement(true);  //because dinamic image

    if(cameraId_ != cameraId)   //if new camera
    {
        if(capture_.isOpened()) //close old camera
            capture_.release();

        //open new camera
        capture_ = cv::VideoCapture(cameraId);

        if(!capture_.isOpened())
        {
            emit error("The camera don`t opened");
            return;
        }

        cameraId_ = cameraId;
    }
}

void SuSearcher::search(cv::Mat*                 image,
                        std::vector<SUPoints>*   points,
                        std::vector<int>*        field,
                        int*                     result)
{
    const int FIRST_FIELD = 0;

    assert(image != 0 && points != 0 && field != 0 && result != 0);
    assert(field->size() == 81);

    *result = SuFindFieldOnFrame::NOT_FOUND;

    std::vector<SUCells>*  cells  = new std::vector<SUCells>;

    cv::Mat  grayImage;

    //1. get image
    switch(source_)
    {
    case IMAGE:
        *image = cv::imread(pathToImage_.toStdString().c_str());

        if(image->empty()){
            emit error("Image don`t opened");
            return;
        }

    break;

    case VIDEO:
        if(!capture_.isOpened())
        {
            emit error("The camera has not been opened");
            return;
        }

        capture_ >> *image;

        if(image->empty()){
            emit error("Can not read image");
            return;
        }

    break;
    }

    //2. to gray image
    cv::cvtColor(*image, grayImage, CV_RGB2GRAY);

    //3. find field
    if(source_ == IMAGE)
        finder_->setCheckOnMovement(false); //because staic frame

    *result = finder_->find(grayImage, *cells, *points);

    if(source_ == IMAGE)
        finder_->setCheckOnMovement(preferences_.isCheckOnMotion);

    assert(*result);

    //send image and frame
    emit searchFieldFinished(image, points, *result);

    if(*result == SuFindFieldOnFrame::CAPTURED &&
       cells->at(FIRST_FIELD).size() > 0)
    {
        //4. recognization
        recognizer_->recognize(cells->at(FIRST_FIELD));

        //5. convert to vector
        unsigned int total = cells->at(FIRST_FIELD).size();

        for(unsigned int i = 0; i < total; i++)
        {
            cv::Point point = cells->at(FIRST_FIELD).at(i)->point();
            int index = point.y * 9 + point.x;
            field->at(index) = cells->at(FIRST_FIELD).at(i)->value();
        }

        emit processEnded();
        return;
    }


    if(source_ == IMAGE)
    {
        emit processEnded();
    }

}

void SuSearcher::clearBuffer()
{
    const int N_FRAMES = 5;

    cv::Mat mat;

    for(int i = 0; i < N_FRAMES; i++)
        capture_ >> mat;
}
