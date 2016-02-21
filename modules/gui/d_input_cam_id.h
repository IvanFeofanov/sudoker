#ifndef D_INPUT_CAM_ID_H
#define D_INPUT_CAM_ID_H

#include <QDialog>

namespace Ui {
class InputCamId;
}

class InputCamId : public QDialog
{
    Q_OBJECT

public:
    explicit InputCamId(int defCameraId, QWidget *parent = 0);
    ~InputCamId();

    int camId();

private:
    Ui::InputCamId *ui;
};

#endif // D_INPUT_CAM_ID_H
