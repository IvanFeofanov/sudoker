#include "d_about.h"
#include "ui_d_about.h"

#include "include/version.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    QString nameAndVersion = QString("<html><center><b> %1 </b> <br> %2 </center></html>")
                             .arg(qApp->applicationName(),
                                  VERSION);
    ui->labelName->setText(nameAndVersion);
}

About::~About()
{
    delete ui;
}
