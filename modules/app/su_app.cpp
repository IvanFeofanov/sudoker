#include "su_app.h"

#include <QDebug>
#include <QTimer>

SuApp::SuApp(SuAppPreferences preferences, QObject *parent) :
    QObject(parent)
{
    qRegisterMetaType<SuAppPreferences>("SuAppPreferences");

    mainWindow_ = new MainWindow();
    worker_     = 0;
    thread_     = 0;

    preferences_ = preferences;

    setup();

    mainWindow_->setup();
    connect(worker_, SIGNAL(finished()),
            this, SLOT(slotQuit()));
//    QTimer::singleShot(5000, this, SIGNAL(stopWorker()));
}
void SuApp::slotQuit()
{
    qDebug() << "slotQuit";
}

SuApp::~SuApp()
{
    emit stopWorker();

    thread_->wait();

    delete mainWindow_;

    qDebug() << "SuApp destruct";
}

void SuApp::updatePreferences(SuAppPreferences preferences)
{
    preferences_ = preferences;

    emit changePreferences(preferences);
}

void SuApp::setup()
{
    worker_     = new SuWorker  (preferences_);
    mainWindow_ = new MainWindow();
    thread_     = new QThread(this);

    worker_->moveToThread(thread_);

    //run worker
    connect(thread_, SIGNAL(started()),
            worker_, SLOT(loop()));

    //quit thread
    connect(worker_, SIGNAL(finished()),
            thread_, SLOT(quit()), Qt::DirectConnection);

    //stop worker
    connect(this,    SIGNAL(stopWorker()),
            worker_, SLOT(stop()));

    //delete worker
    connect(worker_, SIGNAL(finished()),
            worker_, SLOT(deleteLater()));

    //unpause worker
    connect(mainWindow_, SIGNAL(continueCapturing()),
            worker_,     SLOT(unpause()));

    //update preferences
    connect(this,    SIGNAL(changePreferences(SuAppPreferences)),
            worker_, SLOT(updatePreferences(SuAppPreferences)));

    connect(this,        SIGNAL(changePreferences(SuAppPreferences)),
            mainWindow_, SLOT(updatePreferences(SuAppPreferences)));

    connect(mainWindow_, SIGNAL(changePreferences(SuAppPreferences)),
            this,        SLOT(updatePreferences(SuAppPreferences)));

    //video capture
    connect(mainWindow_, SIGNAL(videoCapture(int)),
            worker_,     SLOT(videoProcessing(int)));

    //image from file
    connect(mainWindow_, SIGNAL(imageFromFile(QString)),
            worker_,     SLOT(imageProcessing(QString)));

    //choose mode of solve
    connect(mainWindow_, SIGNAL(selectedSolvingMode(bool)),
            worker_,     SLOT(setSolveMode(bool)));


    //show message
    connect(worker_,     SIGNAL(warning(QString)),
            mainWindow_, SLOT(showMessage(QString)));

    connect(this,        SIGNAL(warning(QString)),
            mainWindow_, SLOT(showMessage(QString)));

    //set image
    connect(worker_,     SIGNAL(searchFieldFinished(cv::Mat*,std::vector<SUPoints>*,int)),
            mainWindow_, SLOT(setImage(cv::Mat*,std::vector<SUPoints>*,int)));

    connect(worker_,     SIGNAL(numberReceived(std::vector<SuFieldCell>*,int)),
            mainWindow_, SLOT(setField(std::vector<SuFieldCell>*,int)));


    //start
    thread_->start();

}

void SuApp::run()
{
    //refresh preferences
    updatePreferences(preferences_);

    mainWindow_->show();
    mainWindow_->autoShowHelp();
}

SuAppPreferences SuApp::preferences()
{
    return preferences_;
}
