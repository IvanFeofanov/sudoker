#include "su_window_settings.h"

SuWindowSettings::SuWindowSettings()
{
    setDefault();
}

void SuWindowSettings::setDefault()
{
    //frame
    colorDinamicFrame  = QColor("#ff0000");
    colorStaticFrame   = QColor("#00ff00");

    widthLineFrame     = 3; //px

    //numbers
    colorNumber         = QColor("#00ff00");
    colorConflictNumber = QColor("#ff0000");

    //paths
    folderForImages = "/home/ivan/program/my/cv/sudoku/pictures";

    cameraId = 0;
}
