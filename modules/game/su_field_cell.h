#ifndef SU_FIELD_CELL_H
#define SU_FIELD_CELL_H

#include <vector>

class SuFieldCell
{
public:
    SuFieldCell();
    SuFieldCell(int val, int tpe);

    int  value;
    int  type;
    bool isConflict;

    enum{
        IN      = 0,
        UNKNOWN = 1,
        SOLVED  = 2
    };

    void conflict   ();
    void noConflict ();
    void resetChange();
    void setNumber  (int val);
};

#endif // SU_FIELD_CELL_H
