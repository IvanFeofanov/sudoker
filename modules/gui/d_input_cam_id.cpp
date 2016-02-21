#include "d_input_cam_id.h"
#include "ui_d_input_cam_id.h"

InputCamId::InputCamId(int defCameraId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputCamId)
{
    ui->setupUi(this);

    ui->lineEdit->setText     (QString::number(defCameraId));
    ui->lineEdit->setValidator(new QIntValidator(0, 99, this));
    ui->lineEdit->setSelection(0, 3);

}

InputCamId::~InputCamId()
{
    delete ui;
}

int InputCamId::camId()
{
    const int DEFAULT_CAMERA_ID = 0;

    bool isOk    = false;
    int cameraId = 0;

    cameraId = ui->lineEdit->text().toInt(&isOk);

    if(isOk)
        return cameraId;
    else
        return DEFAULT_CAMERA_ID;
}
