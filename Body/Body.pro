TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

LIBS += -pthread

SOURCES += main.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    Body.h \
    BodyThreadController.h

