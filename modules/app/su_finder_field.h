#ifndef SU_FINDER_FIELD_H
#define SU_FINDER_FIELD_H

#include <QObject>

#include <cv.h>
#include <highgui.h>

#include "su_app_preferences.h"

#include "su_algorithm.h"

#include "su_abstract_find_field.h"
#include "su_abstract_recognizer.h"
#include "su_find_field_on_frame.h"
#include "su_find_field_on_image.h"
#include "su_am_recognizer.h"

#define DEFAULT_PATH_TO_IMAGE   ("./res/sampleImage.png")
#define DEFAULT_SOURCE          (SuFinderField::IMAGE_FROM_FILE)

class SuFinderField : public QObject
{
    Q_OBJECT
public:
    explicit SuFinderField(QObject *parent = 0,);

signals:
    void handleFinished(cv::Mat*                image,
                        std::vector<SUPoints>*  frame,
                        std::vector<int>*       field);
    void finished      ();
    void warning       (QString text);

public slots:
    void updatePreferences(SuAppPreferences* preferences);

    void imageProcessing  (QString           pathToImage);
    void videoProcessing  (cv::VideoCapture* capture);

    void processing       ();

    int source_;

    enum{
        VIDEO_CAPTURE   = 0,
        IMAGE_FROM_FILE = 1
    };


private:
    SuAppPreferences      preferences_;

    SuFindFieldOnFrame*   finderOnFrame_;
    SuFindFieldOnImage*   finderOnImage_;
    SuAbstractRecognizer* recognizer_;
};

#endif // SU_FINDER_FIELD_H
