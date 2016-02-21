#ifndef D_SET_PREFERENCES_H
#define D_SET_PREFERENCES_H

#include <QDialog>
#include <QFileDialog>

#include "su_app_preferences.h"

namespace Ui {
class SetPreferences;
}

class SetPreferences : public QDialog
{
    Q_OBJECT

public:
    explicit SetPreferences(SuAppPreferences* preferences,
                            QWidget *parent = 0);
    ~SetPreferences();

public:
    SuAppPreferences preferences();

private slots:
    void chosePath();
    void setPath(QString path);
    void setCheckOnMovement(bool isCheck);
    void setSensitivity(int value);

private:
    Ui::SetPreferences *ui;

    SuAppPreferences preferences_;

    static const int MAX_VALUE_SENS_ = 100;
};

#endif // D_SET_PREFERENCES_H
