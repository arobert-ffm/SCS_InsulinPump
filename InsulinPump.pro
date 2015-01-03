#-------------------------------------------------
#
# Project created by QtCreator 2015-01-03T16:46:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = InsulinPump
TEMPLATE = app


SOURCES +=\
    ControlSystem.cpp \
    HormoneReservoir.cpp \
    Pump.cpp \
    Scheduler.cpp \
    Tracer.cpp \
    UserInterface.cpp \
    Main.cpp \
    Body.cpp

HEADERS  += \
    HormoneReservoir.h \
    Pump.h \
    Scheduler.h \
    Tracer.h \
    UserInterface.h \
    ControlSystem.h \
    Body.h

FORMS    += \
    UserInterface.ui
