#-------------------------------------------------
#
# Project created by QtCreator 2014-01-29T19:07:58
#
#-------------------------------------------------

QT       += core gui webkitwidgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = phonograph
TEMPLATE = app


SOURCES += main.cpp\
        phonograph.cpp \
    system/musicdatabase.cpp \
    system/song.cpp

HEADERS  += phonograph.h \
    system/musicdatabase.h \
    system/song.h

FORMS    += phonograph.ui

RESOURCES += \
    icons.qrc \
    theme.qrc
