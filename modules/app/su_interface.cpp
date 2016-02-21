#include "su_interface.h"

SuInterface::SuInterface(QObject *parent) :
    QObject(parent)
{
}

void SuInterface::printField(std::vector<SuFieldCell> field)
{
    std::cout << std::endl;

    for(int y = 0; y < 9; y++)
    {
        for(int x = 0; x < 9; x++)
        {
            int index = y * 9 + x;

            if(field.at(index).type == SuFieldCell::IN)
            {
                if(field.at(index).isConflict)
                    std::cout << "{" << field.at(index).value << "} ";
                else
                    std::cout << "[" << field.at(index).value << "] ";
            }

            if(field.at(index).type == SuFieldCell::SOLVED)
            {
                if(field.at(index).isConflict)
                {
                    std::cout << "{" << field.at(index).value << "} ";
                }
                else
                {
                    std::cout << " " << field.at(index).value << "  ";
                }
            }

            if(field.at(index).type == SuFieldCell::UNKNOWN)
            {
                std::cout <<  "[ ] ";
            }
        }
        std::cout << std::endl;
    }
}
