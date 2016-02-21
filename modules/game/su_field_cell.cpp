#include "su_field_cell.h"

SuFieldCell::SuFieldCell()
{
    value      = 0;
    type       = UNKNOWN;
    isConflict = false;
}

SuFieldCell::SuFieldCell(int val, int tpe)
{
    value      = val;
    type       = value == 0 ? UNKNOWN : tpe;
    isConflict = false;
}

void SuFieldCell::conflict()
{
    isConflict = true;
}

void SuFieldCell::noConflict()
{
    isConflict = false;
}

void SuFieldCell::resetChange()
{
    if(type == SOLVED)
    {
        value       = 0;
        type        = UNKNOWN;
        isConflict  = false;
    }
}

void SuFieldCell::setNumber(int val)
{
    if(type == IN)
        return;

    value = val;
    type  = SOLVED;
}
