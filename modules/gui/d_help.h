#ifndef D_HELP_H
#define D_HELP_H

#include <QDialog>
#include <QSettings>
#include <QString>

#include <tr1/memory>

namespace Ui {
class Help;
}

class Help : public QDialog
{
    Q_OBJECT

public:
    explicit Help(QWidget *parent = 0);
    ~Help();

public slots:
    void autoShow();

private slots:
    void showAgain(bool status);

private:
    Ui::Help *ui;

    QString settingsKey_;
    bool    isShowAgain_;
    std::tr1::shared_ptr<QSettings> settings_;

};

#endif // D_HELP_H
