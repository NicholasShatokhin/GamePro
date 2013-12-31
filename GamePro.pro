#-------------------------------------------------
#
# Project created by QtCreator 2013-12-24T00:26:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GamePro
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    options.cpp \
    statistics.cpp \
    registration.cpp \
    login.cpp \
    game.cpp \
    gamecontroller.cpp \
    clickablewidget.cpp \
    gamefield.cpp

HEADERS  += mainwindow.h \
    options.h \
    statistics.h \
    registration.h \
    login.h \
    game.h \
    gamecontroller.h \
    clickablewidget.h \
    gamefield.h

FORMS    += mainwindow.ui \
    options.ui \
    statistics.ui \
    registration.ui \
    login.ui \
    game.ui

RESOURCES += \
    resources.qrc
