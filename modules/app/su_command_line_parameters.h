#ifndef SU_COMMAND_LINE_PARAMETERS_H
#define SU_COMMAND_LINE_PARAMETERS_H

#include <QString>

class SuCommandLineParameters
{
public:
    bool    showHelp;
    bool    showVersion;
    QString unknownParameters;
};

#endif // SU_COMMAND_LINE_PARAMETERS_H
