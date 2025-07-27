QT       += core gui widgets multimedia

CONFIG += c++11

# Deprecated API warnings
DEFINES += QT_DEPRECATED_WARNINGS

TARGET = JingZiQiEvo
TEMPLATE = app

# Resource file
RESOURCES += \
    resourcejzq.qrc

# Source files
SOURCES += \
    StartScreen.cpp \
    main.cpp \
    mylabel.cpp \
    widget.cpp

# Header files
HEADERS += \
    StartScreen.h \
    mylabel.h \
    widget.h
