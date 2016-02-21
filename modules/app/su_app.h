#ifndef SU_APP_H
#define SU_APP_H

#include <QObject>
#include <QThread>
#include <QTimer>

#include "su_app_preferences.h"
#include "su_window_settings.h"

#include "w_main_window.h"
#include "su_worker.h"

class SuApp : public QObject
{
    Q_OBJECT
public:
    explicit SuApp(SuAppPreferences preferences,
                   QObject*         parent = 0);
    ~SuApp();

    SuAppPreferences preferences();

signals:
    void changePreferences(SuAppPreferences  preferences);
    void warning          (QString           text);
    void videoCaptured    (cv::VideoCapture* capture);

    void stopWorker       ();

    void choose();

private slots:


public slots:
    void run ();

    void updatePreferences(SuAppPreferences preferences);

private:
    SuAppPreferences preferences_;

    MainWindow* mainWindow_;
    SuWorker*   worker_;
    QThread*    thread_;

    cv::VideoCapture    capture_;

    void setup();

};

#endif // SU_APP_H
