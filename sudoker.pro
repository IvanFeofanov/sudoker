
TARGET = sudoker

DESTDIR     = bin
OBJECTS_DIR = objects_dir
MOC_DIR     = moc_dir
RCC_DIR     = rcc_dir
UI_DIR      = ui_dir

#opencv
unix: {
    INCLUDEPATH += /usr/local/include/opencv
    INCLUDEPATH += /usr/local/lib
    LIBS        += -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_gpu -lopencv_ml
}

win32 {
    INCLUDEPATH += C:/opencv-2.4.9/opencv/build/install/include/opencv
    INCLUDEPATH += C:/opencv-2.4.9/opencv/build/install/include
    LIBS += C:/opencv-2.4.9/opencv/build/bin/libopencv_core249.dll
    LIBS += C:/opencv-2.4.9/opencv/build/bin/libopencv_highgui249.dll
    LIBS += C:/opencv-2.4.9/opencv/build/bin/libopencv_imgproc249.dll
}

DEPENDPATH *= $$PATHS

include($$PWD/modules/app/app.pro)
include($$PWD/modules/capture/capture.pro)
include($$PWD/modules/game/game.pro)
include($$PWD/modules/gui/gui.pro)

HEADERS += $$PWD/include/version.h

#VERSION = 2.5.5


RESOURCES += \
    ./res/doc.qrc
