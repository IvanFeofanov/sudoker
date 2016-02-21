#ifndef D_ABOUT_H
#define D_ABOUT_H

#include <QDialog>

namespace Ui {
class About;
}

class About : public QDialog
{
    Q_OBJECT

public:
    explicit About(QWidget *parent = 0);
    ~About();

private:
    Ui::About *ui;
};

#endif // D_ABOUT_H
