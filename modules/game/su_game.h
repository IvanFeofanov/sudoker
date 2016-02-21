#ifndef SU_GAME_H
#define SU_GAME_H

#include <iostream>
#include <cassert>

#include <vector>
#include <algorithm>

#include <QObject>
#include <QSet>

#include "su_field_cell.h"

class SuGame : public QObject
{
    Q_OBJECT
public:
    SuGame(QObject* parent);
    SuGame(std::vector<int>& field, QObject *parent = 0);

    static bool checkOnConflict(std::vector<SuFieldCell>& field);
    static bool checkOnWinning (std::vector<SuFieldCell>& field);

    static int  solve(std::vector<SuFieldCell>& field);

    static std::vector<SuFieldCell> toFieldCell(std::vector<int>& field);

    enum{
        SOLVED     = 2,
        NOT_SOLVED = 1,
        ERROR      = 0
    };

    static const int LEN_SIDE_OF_FIELD = 9;
    static const int FIELD_SIZE        = 81;

signals:
    void changed    (std::vector<SuFieldCell> field);
    void winning    ();

public slots:
    void setField       (std::vector<int>& field);
    void clearChanges   ();
    void setNumber      (int col, int row, int number);

private:
    std::vector<SuFieldCell> src_field_;

    static QSet<int> findPossibleValues_(std::vector<SuFieldCell> &field, int x, int y);
    static QSet<int> getRowOfFiled_  (std::vector<SuFieldCell> &field, int y);
    static QSet<int> getColOfField_  (std::vector<SuFieldCell> &field, int x);
    static QSet<int> getBlockOfField_(std::vector<SuFieldCell> &field, int x, int y);

    static int solve_(std::vector<SuFieldCell>& field);

};

#endif // SU_GAME_H
