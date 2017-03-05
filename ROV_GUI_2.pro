#-------------------------------------------------
#
# Project created by QtCreator 2017-01-29T09:46:51
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ROV_GUI_2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    arduino.cpp \
    compass.cpp \
    joystick.cpp \
    passworddialog.cpp

HEADERS  += mainwindow.h \
    arduino.h \
    compass.h \
    joystick.h \
    passworddialog.h

FORMS    += mainwindow.ui \
    passworddialog.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/E:/SDL2-2.0.5/lib/x64/ -lSDL2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/E:/SDL2-2.0.5/lib/x64/ -lSDL2d
else:unix: LIBS += -L$$PWD/E:/SDL2-2.0.5/lib/x64/ -lSDL2

INCLUDEPATH += $$PWD/E:/SDL2-2.0.5/lib/x64
DEPENDPATH += $$PWD/E:/SDL2-2.0.5/lib/x64

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/E:/SDL2-2.0.5/lib/x64/ -lSDL2main
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/E:/SDL2-2.0.5/lib/x64/ -lSDL2maind
else:unix: LIBS += -L$$PWD/E:/SDL2-2.0.5/lib/x64/ -lSDL2main

INCLUDEPATH += $$PWD/E:/SDL2-2.0.5/lib/x64
DEPENDPATH += $$PWD/E:/SDL2-2.0.5/lib/x64

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/E:/SDL2-2.0.5/lib/x64/ -lSDL2test
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/E:/SDL2-2.0.5/lib/x64/ -lSDL2testd
else:unix: LIBS += -L$$PWD/E:/SDL2-2.0.5/lib/x64/ -lSDL2test

INCLUDEPATH += $$PWD/E:/SDL2-2.0.5/lib/x64
DEPENDPATH += $$PWD/E:/SDL2-2.0.5/lib/x64

DISTFILES += \
    IPAddress.txt

win32:RC_ICONS += Logo.ico



