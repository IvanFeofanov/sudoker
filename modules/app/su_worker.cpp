#include "su_worker.h"

#include <QDebug>

SuWorker::SuWorker(SuAppPreferences preferences, QObject *parent) :
    QObject(parent)
{
    searcher_ = new SuSearcher(preferences, this);

    //handle error
    connect(searcher_, SIGNAL(error(QString)),
            this, SIGNAL(warning(QString)));

    connect(searcher_, SIGNAL(error(QString)),
            this, SLOT(pause()));

    connect(searcher_, SIGNAL(warning(QString)),
            this, SIGNAL(warning(QString)));

    //process ended
    connect(searcher_, SIGNAL(processEnded()),
            this, SLOT(pause()));

    //send image and frame
    connect(searcher_, SIGNAL(searchFieldFinished(cv::Mat*,std::vector<SUPoints>*,int)),
            this, SIGNAL(searchFieldFinished(cv::Mat*,std::vector<SUPoints>*,int)));

    updatePreferences(preferences);

    isRun_ = true;
    pause();
}

SuWorker::~SuWorker()
{
    delete searcher_;
}

void SuWorker::updatePreferences(SuAppPreferences preferences)
{
    preferences_ = preferences;

    searcher_->updatePreferences(preferences);
}

void SuWorker::loop()
{
    if(isRun_ && !isPause_)
    {
        run();
        QTimer::singleShot(1, this, SLOT(loop()));
    }

    if(!isRun_)
    {
        emit finished();
        return;
    }

    if(isPause_)
    {
        QTimer::singleShot(1, this, SLOT(loop()));
    }

}

void SuWorker::run()
{
    cv::Mat*                  image     = new cv::Mat();
    std::vector<SUPoints>*    frame     = new std::vector<SUPoints>;
    std::vector<SuFieldCell>* gameField = new std::vector<SuFieldCell>;
    std::vector<int>          field     = std::vector<int>(81);

    int searchStatus = SuFindFieldOnFrame::NOT_FOUND;
    int solveStatus  = SuGame::NOT_SOLVED;

    //1. search
    searcher_->search(image, frame, &field, &searchStatus);

    if(searchStatus == SuFindFieldOnFrame::CAPTURED)
    {
        //2. get the game field
        *gameField = SuGame::toFieldCell(field);

        //3. solve
        if(isSolve_)
        {
            int status = SuGame::NOT_SOLVED;

            status = SuGame::solve(*gameField);

            switch(status)
            {
            case SuGame::NOT_SOLVED:
                emit warning("The solution is not found");
                break;
            case SuGame::ERROR:
                emit warning("Error in finding a solution");
                break;
            }
        }

        //4. send field
        emit numberReceived(gameField, solveStatus);
    }
}

void SuWorker::setSolveMode(bool status)
{
    isSolve_ = status;
}

void SuWorker::imageProcessing(QString pathToImage)
{
    searcher_->imageProcessing(pathToImage);
    unpause();
}

void SuWorker::videoProcessing(int cameraId)
{
    searcher_->videoProcessing(cameraId);
    unpause();
}

void SuWorker::release()
{
    delete searcher_;
}

void SuWorker::pause()
{
    isPause_ = true;
}

void SuWorker::unpause()
{
    isPause_ = false;

    searcher_->clearBuffer();
}

void SuWorker::stop()
{
    isRun_ = false;
}
