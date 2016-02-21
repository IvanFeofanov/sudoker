#include "d_set_preferences.h"
#include "ui_d_set_preferences.h"

SetPreferences::SetPreferences(SuAppPreferences* preferences,
                               QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetPreferences)
{
    ui->setupUi(this);

    preferences_ = *preferences;

    //check on movement
    ui->checkBoxMovement->setCheckState(preferences_.isCheckOnMotion ? Qt::Checked :
                                                                       Qt::Unchecked);
    ui->labelSensitivity->setEnabled(preferences_.isCheckOnMotion);

    ui->horizontalSliderSensitivity->setEnabled(preferences_.isCheckOnMotion);
    ui->horizontalSliderSensitivity->setRange(0, MAX_VALUE_SENS_);
    ui->horizontalSliderSensitivity->setValue((int)(preferences_.sensitivityToMovement * MAX_VALUE_SENS_));

    ui->lineEditPath->setText(QString::fromStdString(preferences_.pathToTrainData));

    //sensitivity
    connect(ui->checkBoxMovement, SIGNAL(clicked(bool)),
            this, SLOT(setCheckOnMovement(bool)));

    connect(ui->horizontalSliderSensitivity, SIGNAL(valueChanged(int)),
            this, SLOT(setSensitivity(int)));

    //path to train data
    connect(ui->lineEditPath, SIGNAL(textEdited(QString)),
            this, SLOT(setPath(QString)));

    connect(ui->toolButtonPath, SIGNAL(clicked()),
            this, SLOT(chosePath()));
}

SetPreferences::~SetPreferences()
{
    delete ui;
}

void SetPreferences::setCheckOnMovement(bool isCheck)
{
    preferences_.isCheckOnMotion = isCheck;
}

void SetPreferences::setSensitivity(int value)
{
    preferences_.sensitivityToMovement = (float)value / MAX_VALUE_SENS_;
}

void SetPreferences::chosePath()
{
    //get file name
    QFileInfo file(QString::fromStdString(preferences_.pathToTrainData));

    //get file name
    QString path = QFileDialog::getOpenFileName( this,
                                                 "Chose data train file",
                                                 file.absoluteDir().path(),
                                                 "*.xml");
    if(!path.isEmpty())
    {
        //save
        preferences_.pathToTrainData = path.toStdString();

        //set text to line edit
        ui->lineEditPath->setText(path);
    }
}

void SetPreferences::setPath(QString path)
{
    if(!path.isEmpty())
        if(QFile::exists(path))
            preferences_.pathToTrainData = path.toStdString();
}

SuAppPreferences SetPreferences::preferences()
{
    return preferences_;
}
