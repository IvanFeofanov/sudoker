
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

PATHS *= \
    $$PWD

DEPENDPATH *= $$PATHS

INCLUDEPATH *= $$PATHS

FORMS += \
    $$PWD/w_main_window.ui \
    $$PWD/d_input_cam_id.ui \
    $$PWD/d_set_preferences.ui \
    $$PWD/d_about.ui \
    $$PWD/d_help.ui

HEADERS += \
    $$PWD/w_main_window.h \
    $$PWD/su_image_converter.h \
    $$PWD/d_input_cam_id.h \
    $$PWD/d_set_preferences.h \
    $$PWD/su_window_settings.h \
    $$PWD/d_about.h \
    $$PWD/d_help.h

SOURCES += \
    $$PWD/w_main_window.cpp \
    $$PWD/su_image_converter.cpp \
    $$PWD/d_input_cam_id.cpp \
    $$PWD/d_set_preferences.cpp \
    $$PWD/su_window_settings.cpp \
    $$PWD/d_about.cpp \
    $$PWD/d_help.cpp

RESOURCES += \
    $$PWD/images.qrc

OTHER_FILES += \
    $$PWD/icons/abouLogo.png
