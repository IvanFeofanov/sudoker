#ifndef SU_SEARCHER_H
#define SU_SEARCHER_H

#include <QObject>

#include <cv.h>
#include <highgui.h>

#include "su_app_preferences.h"

#include "su_algorithm.h"

#include "finder/su_find_field_on_frame.h"
#include "finder/noname_algorithm/su_noname_finder.h"

#include "recognizer/su_am_recognizer.h"

#define PATH_TO_DEFAULT_IMAGE ("./res/sample.png")

class SuSearcher : public QObject
{
    Q_OBJECT
public:
    explicit SuSearcher(SuAppPreferences preferences,
                        QObject *parent = 0);
    SuSearcher();
    ~SuSearcher();

    void search (cv::Mat*                 image,
                 std::vector<SUPoints>*   points,
                 std::vector<int>*        field,
                 int*                     status);

signals:
    void finished           ();
    void error              (QString message);
    void processEnded       ();
    void warning            (QString message);
    void searchFieldFinished(cv::Mat*                  image,
                             std::vector<SUPoints>*    frame,
                             int                       searchStatus);

public slots:
    void imageProcessing  (QString          pathToImage);
    void videoProcessing  (int              cameraId);
    void updatePreferences(SuAppPreferences preferences);
    void clearBuffer();

private:
    int source_;

    enum{
        VIDEO = 0,
        IMAGE = 1
    };

    SuFindFieldOnFrame* finder_;
    SuAMRecognizer*     recognizer_;

    cv::VideoCapture    capture_;
    int                 cameraId_;

    QString             pathToImage_;

    SuAppPreferences    preferences_;
};

#endif // SU_SEARCHER_H
