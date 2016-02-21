#include "su_app_preferences.h"

#include <iostream>

//default preferences
SuAppPreferences::SuAppPreferences()
{
    setDefault();
}

SuAppPreferences::SuAppPreferences(QSettings &settings)
{
    setDefault();
    read(settings);
}

void SuAppPreferences::setDefault()
{
    //recognizer
    pathToTrainData = "./res/train_data/trainData.xml";

    //video
    isCheckOnMotion = true;
    sensitivityToMovement = 0.3;

    //keys
    pathToTrainDataKey_       = "pathToTrainData";
    isCheckOnMotionKey_       = "isCheckOnMotion";
    sensitivityToMovementKey_ = "sensitivityToMovement";
}

void SuAppPreferences::read(QSettings &settings)
{
    //path to train data
    pathToTrainData = settings.value(pathToTrainDataKey_, QString::fromStdString(pathToTrainData)).toString().toStdString();

    //is check on motion
    isCheckOnMotion = settings.value(isCheckOnMotionKey_, isCheckOnMotion).toBool();

    //sensitivity to movement
    sensitivityToMovement = settings.value(sensitivityToMovementKey_, sensitivityToMovement).toFloat();

}

void SuAppPreferences::write(QSettings &settings)
{
    //path to train data
    settings.setValue(pathToTrainDataKey_, QString(pathToTrainData.c_str()));

    //is check on motion
    settings.setValue(isCheckOnMotionKey_, isCheckOnMotion);

    //sensitivity to movement
    settings.setValue(sensitivityToMovementKey_, sensitivityToMovement);
}
