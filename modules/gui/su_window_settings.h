#ifndef SU_WINDOW_SETTINGS_H
#define SU_WINDOW_SETTINGS_H

#include <QColor>

class SuWindowSettings
{
public:
    SuWindowSettings();

    //frame
    QColor colorDinamicFrame;
    QColor colorStaticFrame;

    int widthLineFrame;

    //numbers
    QColor colorNumber;
    QColor colorConflictNumber;

    //paths
    QString folderForImages;
    int cameraId;

    void setDefault();
};

#endif // SU_WINDOW_SETTINGS_H
