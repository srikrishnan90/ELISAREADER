#-------------------------------------------------
#
# Project created by QtCreator 2023-09-20T10:33:42
#
#-------------------------------------------------

QT       += core gui sql qml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ELISAREADER
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    keyboard.cpp \
    ADS1263.cpp \
    DEV_Config.cpp \
    pi2c.cpp \
    regressionline.cpp

HEADERS += \
        mainwindow.h \
    keyboard.h \
    ADS1263.h \
    DEV_Config.h \
    pi2c.h \
    regressionline.h

FORMS += \
        mainwindow.ui \
    keyboard.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH    += /usr/local/include
LIBS += -L/usr/local/lib -lwiringPi
