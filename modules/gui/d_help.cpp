#include "d_help.h"
#include "ui_d_help.h"
#include <iostream>

Help::Help(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Help)
{
    ui->setupUi(this);

    //open settings
    settingsKey_ = "/settings/help/showAgain";
    settings_ = std::tr1::shared_ptr<QSettings>(new QSettings(qApp->organizationName(),
                                                              qApp->applicationName()));

    //read settings
    if(settings_->contains(settingsKey_))
    {
        isShowAgain_ = settings_->value(settingsKey_).toBool();

        ui->checkBoxShowAgain->setCheckState(isShowAgain_ ? Qt::Checked :
                                                            Qt::Unchecked);
    }
    else
    {
        isShowAgain_ = true;
        ui->checkBoxShowAgain->setCheckState(Qt::Checked);
    }

    //
    connect(this->ui->checkBoxShowAgain, SIGNAL(clicked(bool)),
            this, SLOT(showAgain(bool)));

    //set help page
    ui->textBrowser->setSearchPaths(QStringList() << ":");
    ui->textBrowser->setSource(QString("doc/help_page.html"));

}

Help::~Help()
{
    delete ui;
}

void Help::autoShow()
{
    if(isShowAgain_)
        this->show();
}

void Help::showAgain(bool status)
{
    isShowAgain_ = status;

    settings_->setValue(settingsKey_, isShowAgain_);
}
