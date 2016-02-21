#include <iostream>
#include <math.h>
#include <vector>

#include <QtCore>
#include <QApplication>

#include <cv.h>
#include <highgui.h>

#include "su_app.h"
#include "su_app_preferences.h"
#include "su_command_line_parameters.h"

#include "./include/version.h"

void printHelpMessage();
void printVersion();
SuCommandLineParameters parseCommandLine();
void printError         (const QString errorMessage);
void readAppPreferences (SuAppPreferences &preferences);
void writeAppPreferences(SuAppPreferences &preferences);

int main(int argc, char** argv)
{
    QApplication application(argc, argv);

    application.setOrganizationName("Crutches and Cycles co");
    application.setApplicationName ("Sudoker");

    // 1.
    SuCommandLineParameters parameters;
    parameters = parseCommandLine();

    //unknow parameters
    if(!parameters.unknownParameters.isEmpty())
    {
        QString errorMessage;
        QTextStream(&errorMessage) << qApp->arguments()[0] \
                                   << ": invalid option '" \
                                   << parameters.unknownParameters \
                                   << "' \n" \
                                   << "Try '" << qApp->arguments()[0] \
                                   << " --help' for more information.\n";

        printError(errorMessage);

        return -1;
    }

    //help
    if(parameters.showHelp)
    {
        printHelpMessage();
        return 0;
    }

    //version
    if(parameters.showVersion)
    {
        printVersion();
        return 0;
    }

    // 2. read preferences
    SuAppPreferences preferences;
    readAppPreferences(preferences);

    // 3. create the object of game
    SuApp sudokuApp(preferences);

    // 4. run
    sudokuApp.run();
    int exitCode = application.exec();

    // 5. write app prefferences
    preferences = sudokuApp.preferences();
    writeAppPreferences(preferences);

    return exitCode;
}

void readAppPreferences(SuAppPreferences &preferences)
{
    QSettings settings(qApp->organizationName(),
                       qApp->applicationName());

    preferences = SuAppPreferences(settings);
}

void writeAppPreferences(SuAppPreferences &preferences)
{
    QSettings settings(qApp->organizationName(),
                       qApp->applicationName());

    preferences.write(settings);
}

void printError(const QString errorMessage)
{
    std::cout << errorMessage.toStdString() << std::endl;
}

void printHelpMessage()
{
    QString helpMessage;

    helpMessage += "\n";
    helpMessage += "    -h, --help      give this help list\n";
    helpMessage += "    -v, --version   print program version\n";

    std::cout << helpMessage.toStdString() << std::endl;
}

void printVersion()
{
    std::cout << qApp->applicationName().toStdString() \
              << " " << VERSION << std::endl;
}

SuCommandLineParameters parseCommandLine()
{
    SuCommandLineParameters resultParameters;
    QStringList arguments = qApp->arguments();
    QStringList::ConstIterator arg;

    //default
    resultParameters.showHelp = false;
    resultParameters.showVersion = false;
    resultParameters.unknownParameters = "";

    //for each argumet
    for(arg = arguments.cbegin(); arg != arguments.cend(); ++arg)
    {
        if((*arg).startsWith("--") ||
           ((*arg).startsWith("-") && (*arg).size() == 2))
        {
            //parse know parameters
            if(((*arg) == "-h") || ((*arg) == "--help"))
            {
                resultParameters.showHelp = true;
            }
            else if(((*arg) == "-V") || ((*arg) == "--version"))
            {
                resultParameters.showVersion = true;
            }

            //unknown argument
            else{
                resultParameters.unknownParameters = *arg;
                break;
            }
        }
    }

    return resultParameters;
}
