TEMPLATE = app

SOURCES *= \
    $$PWD/main.cpp \
#    $$PWD/su_interface.cpp \
    $$PWD/su_app_preferences.cpp \
    $$PWD/su_app.cpp \
    $$PWD/su_worker.cpp \
#    $$PWD/su_finder_field.cpp
    $$PWD/su_searcher.cpp \
    $$PWD/su_program_state.cpp

PATHS *= \
    $$PWD

DEPENDPATH *= $$PATHS

INCLUDEPATH *= $$PATHS

HEADERS += \
#    $$PWD/su_interface.h \
    $$PWD/su_app_preferences.h \
    $$PWD/su_app.h \
    $$PWD/su_worker.h \
#    $$PWD/su_finder_field.h
    $$PWD/su_searcher.h \
    $$PWD/su_program_state.h \
    $$PWD/su_command_line_parameters.h
