#ifndef SU_APP_PREFERENCES_H
#define SU_APP_PREFERENCES_H

#include <QtCore>

#include <string>

class SuAppPreferences
{
public:
    SuAppPreferences();
    SuAppPreferences(QSettings &settings);

    void write(QSettings &settings);
    void read (QSettings &settings);

    void setDefault();

    //recognizer
    std::string pathToTrainData;

    //video
    bool  isCheckOnMotion;
    float sensitivityToMovement;

private:

    QString pathToTrainDataKey_;
    QString isCheckOnMotionKey_;
    QString sensitivityToMovementKey_;
};

#endif // SU_APP_PREFERENCES_H
