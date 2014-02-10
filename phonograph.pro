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
    system/song.cpp \
    system/qsongitem.cpp \
    system/qplaylistitem.cpp

HEADERS  += phonograph.h \
    system/musicdatabase.h \
    system/song.h \
    system/qsongitem.h \
    system/qplaylistitem.h

FORMS    += phonograph.ui

RESOURCES += \
    icons.qrc \
    theme.qrc

OTHER_FILES +=
