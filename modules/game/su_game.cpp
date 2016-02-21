#include "su_game.h"

SuGame::SuGame(std::vector<int>& field, QObject *parent) :
    QObject(parent)
{
    setField(field);

}

SuGame::SuGame(QObject* parent) :
    QObject(parent)
{
}

void SuGame::setField(std::vector<int> &field)
{
    src_field_ = toFieldCell(field);

    emit changed(src_field_);
}

void SuGame::clearChanges()
{
    assert(src_field_.size() == FIELD_SIZE);

    for(int i = 0; i < FIELD_SIZE; i++)
    {
        src_field_.at(i).resetChange();
    }

    emit changed(src_field_);
}

void SuGame::setNumber(int col, int row, int number)
{
    assert(number > 0 && number > 9);
    assert(col > 0 && col < 9 && row > 0 && row < 9);
    assert(src_field_.size() == FIELD_SIZE);

    src_field_.at(LEN_SIDE_OF_FIELD * row + col).setNumber(number);

    if(!checkOnConflict(src_field_))
        if(checkOnWinning(src_field_))
            emit winning();

    changed(src_field_);
}

bool SuGame::checkOnConflict(std::vector<SuFieldCell>& field)
{
    assert(field.size() == FIELD_SIZE);

    bool isConflict = false;

    std::vector<std::vector<int> > accumulator;

    //zero
    for(int i = 0; i < FIELD_SIZE; i++)
        field.at(i).noConflict();

    //check on all colls
    for(int col = 0; col < LEN_SIDE_OF_FIELD; col++)
    {
        accumulator = std::vector<std::vector<int> >(LEN_SIDE_OF_FIELD);

        for(int row = 0; row < LEN_SIDE_OF_FIELD; row++)
        {
            int index = col * LEN_SIDE_OF_FIELD + row;
            int value = field.at(index).value;

            if(value == 0)
                continue;

            accumulator.at(value - 1).push_back(index);

            if(accumulator.at(value - 1).size() > 1)
            {
                isConflict = true;

                for(unsigned int i = 0; i < accumulator.at(value - 1).size(); i++)
                    field.at(accumulator.at(value - 1).at(i)).conflict();
            }
        }
    }

    //check on all rows
    for(int row = 0; row < LEN_SIDE_OF_FIELD; row++)
    {
        accumulator = std::vector<std::vector<int> >(LEN_SIDE_OF_FIELD);

        for(int col = 0; col < LEN_SIDE_OF_FIELD; col++)
        {
            int index = row * LEN_SIDE_OF_FIELD + col;
            int value = field.at(index).value;

            if(value == 0)
                continue;

            accumulator.at(value - 1).push_back(index);

            if(accumulator.at(value - 1).size() > 1)
            {
                isConflict = true;

                for(unsigned int i  = 0; i < accumulator.at(value - 1).size(); i++)
                    field.at(accumulator.at(value - 1).at(i)).conflict();
            }
        }
    }


    //check on all rects
    for(int x = 0; x < LEN_SIDE_OF_FIELD; x += 3)
    {
        for(int y = 0; y < LEN_SIDE_OF_FIELD; y += 3)
        {
            accumulator = std::vector<std::vector<int> >(LEN_SIDE_OF_FIELD);

            for(int j = x; j < x+3; j++)
            {
                for(int i = y; i < y+3; i++)
                {
                    int index = j * LEN_SIDE_OF_FIELD + i;
                    int value = field.at(index).value;

                    if(value == 0)
                        continue;

                    accumulator.at(value - 1).push_back(index);

                    if(accumulator.at(value - 1).size() > 1)
                    {
                        isConflict = true;

                        for(unsigned int h = 0; h < accumulator.at(value - 1).size(); h++)
                            field.at(accumulator.at(value - 1).at(h)).conflict();
                    }
                }
            }
        }
    }

    return isConflict;
}

bool SuGame::checkOnWinning(std::vector<SuFieldCell> &field)
{
    assert(field.size() == FIELD_SIZE);

    if(checkOnConflict(field))
    {
        return false;
    }

    for(int i = 0; i < FIELD_SIZE; i++)
    {
        if(field.at(i).value == 0)
        {
            return false;
        }
    }

    return true;
}

std::vector<SuFieldCell> SuGame::toFieldCell(std::vector<int> &field)
{
    assert(field.size() == FIELD_SIZE);

    std::vector<SuFieldCell> gameField;

    for(int i = 0; i < FIELD_SIZE; i++)
    {
        SuFieldCell cell(field.at(i),
                         field.at(i) == 0 ? SuFieldCell::UNKNOWN : SuFieldCell::IN);
        gameField.push_back(cell);
    }

    checkOnConflict(gameField);

    return gameField;
}

int SuGame::solve(std::vector<SuFieldCell> &field)
{
    std::vector<SuFieldCell> fieldCopy = field;

    if(checkOnConflict(field))
        return NOT_SOLVED;

    int status = solve_(fieldCopy);

    if(status == SOLVED)
        field = fieldCopy;

    return status;
}

int SuGame::solve_(std::vector<SuFieldCell> &field)
{
    int minRow = -1;
    int minCol = -1;

    QSet<int> minValues;

    while(true)
    {
        minRow = -1;

        for(int y = 0; y < 9; y++)
        {
            for(int x = 0; x < 9; x++)
            {
                int index = y * 9 + x;

                if(field.at(index).value != 0)
                    continue;

                QSet<int> possibleValues = findPossibleValues_(field, x, y);

                int possibleValuesCount = possibleValues.size();

                if(possibleValuesCount == 0)
                    return NOT_SOLVED;

                if(possibleValuesCount == 1)
                    field[index].setNumber(*possibleValues.begin());

                if(minRow < 0 || possibleValuesCount < minValues.size())
                {
                    minRow = y;
                    minCol = x;
                    minValues = possibleValues;
                }
            }
        }

        if(minRow == -1)
        {
            return SOLVED;
        }
        else if(minValues.size() > 1)
        {
            break;
        }
    }

    for(QSet<int>::iterator v = minValues.begin(); v != minValues.end(); ++v)
    {
        std::vector<SuFieldCell> fieldCopy = field;
        fieldCopy[minRow * LEN_SIDE_OF_FIELD + minCol].setNumber(*v);

        if(solve_(fieldCopy) == SOLVED)
        {
            field = fieldCopy;
            return SOLVED;
        }
    }

    return NOT_SOLVED;
}

QSet<int> SuGame::findPossibleValues_(std::vector<SuFieldCell> &field, int x, int y)
{
    QSet<int> possibleValues;
    for(int i = 1; i < 10; i++)
        possibleValues.insert(i);

    possibleValues -= getColOfField_  (field, x);
    possibleValues -= getRowOfFiled_  (field, y);
    possibleValues -= getBlockOfField_(field, x, y);

    return possibleValues;
}

QSet<int> SuGame::getRowOfFiled_(std::vector<SuFieldCell> &field, int y)
{
    QSet<int> row;

    int beginIndex = y * LEN_SIDE_OF_FIELD;

    for(int i = 0; i < LEN_SIDE_OF_FIELD; i++)
    {
        row.insert(field.at(beginIndex + i).value);
    }

    row.remove(0);

    return row;
}

QSet<int> SuGame::getColOfField_(std::vector<SuFieldCell> &field, int x)
{
    QSet<int> col;

    for(int i = 0; i < LEN_SIDE_OF_FIELD; i++)
    {
        col.insert(field.at(i * LEN_SIDE_OF_FIELD + x).value);
    }

    col.remove(0);

    return col;
}

QSet<int> SuGame::getBlockOfField_(std::vector<SuFieldCell> &field, int x, int y)
{
    QSet<int> block;

    int beginCol = (x / 3) * 3;
    int beginRow = (y / 3) * 3;

    for(int row = 0; row < 3; row++)
    {
        for(int col = 0; col < 3; col++)
        {
            block.insert(field[(beginRow + row) * LEN_SIDE_OF_FIELD + beginCol + col].value);
        }
    }

    block.remove(0);

    return block;
}
