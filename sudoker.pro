
TARGET = sudoker

DESTDIR     = bin
OBJECTS_DIR = objects
MOC_DIR     = moc_dir
RCC_DIR     = rcc_dir
UI_DIR      = ui_dir

#opencv
INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/local/lib
LIBS        += -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_gpu -lopencv_ml

DEPENDPATH *= $$PATHS

include($$PWD/modules/app/app.pro)
include($$PWD/modules/capture/capture.pro)
include($$PWD/modules/game/game.pro)
include($$PWD/modules/gui/gui.pro)

HEADERS += $$PWD/include/version.h

#VERSION = 2.5.5

#packagesExist(opencv)

RESOURCES += \
    ./res/doc.qrc
