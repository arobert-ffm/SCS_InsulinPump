#-------------------------------------------------
#
# Project created by QtCreator 2015-01-03T16:46:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = InsulinPump
TEMPLATE = app

CONFIG += c++11

LIBS += -pthread

SOURCES +=\
    ControlSystem.cpp \
    Pump.cpp \
    Scheduler.cpp \
    Tracer.cpp \
    UserInterface.cpp \
    main.cpp

HEADERS  += \
    Pump.h \
    Scheduler.h \
    Tracer.h \
    UserInterface.h \
    ControlSystem.h \
    Config.h

FORMS    += \
    UserInterface.ui

RESOURCES += \
    Ressources.qrc

#QMAKE_POST_LINK = cp ../*.conf ./; cp ../*-Body-*/Body ./

