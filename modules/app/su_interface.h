/*
 * console interface for output data
 */

#ifndef SU_INTERFACE_H
#define SU_INTERFACE_H

#include <vector>
#include <iostream>

#include <QObject>

#include "su_field_cell.h"

class SuInterface : public QObject
{
    Q_OBJECT
public:
    explicit SuInterface(QObject *parent = 0);

signals:

public slots:
    void printField(std::vector<SuFieldCell> field);
};

#endif // SU_INTERFACE_H
