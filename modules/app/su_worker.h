#ifndef SU_WORKER_H
#define SU_WORKER_H

#include <QObject>
#include <QTimer>

#include "cv.h"
#include "highgui.h"

#include "su_app_preferences.h"

#include "su_algorithm.h"

#include "su_game.h"
#include "su_searcher.h"

class SuWorker : public QObject
{
    Q_OBJECT
public:
    explicit SuWorker(SuAppPreferences preferences,
                      QObject *parent = 0);

    ~SuWorker();

signals:
    void finished       ();
    void searchFieldFinished (cv::Mat*                  image,
                              std::vector<SUPoints>*    frame,
                              int                       searchStatus);
    void numberReceived      (std::vector<SuFieldCell>* field,
                              int                       solveStatus);

    void warning(QString text);

public slots:
    void updatePreferences (SuAppPreferences preferences);

    void imageProcessing   (QString           pathToImage);
    void videoProcessing   (int               cameraId);
    void setSolveMode      (bool              status);

    void loop();
    void pause();
    void unpause();
    void stop();

    void release();

private:
    void run();

    SuSearcher*      searcher_;
    bool             isSolve_;

    SuAppPreferences preferences_;

    bool isRun_;
    bool isPause_;
};

#endif // SU_WORKER_H
