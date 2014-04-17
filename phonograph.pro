#-------------------------------------------------
#
# Project created by QtCreator 2014-01-29T19:07:58
#
#-------------------------------------------------

QT       += core gui webkitwidgets sql multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = phonograph
TEMPLATE = app


SOURCES += main.cpp\
        phonograph.cpp \
    system/musicdatabase.cpp \
    system/song.cpp \
    system/qsongitem.cpp \
    system/qplaylistitem.cpp \
    system/databasecache.cpp \
    system/qplaylist.cpp \
    aboutdialog.cpp \
    login.cpp \
    system/authentication.cpp

HEADERS  += phonograph.h \
    system/musicdatabase.h \
    system/song.h \
    system/qsongitem.h \
    system/qplaylistitem.h \
    system/databasecache.h \
    system/qplaylist.h \
    aboutdialog.h \
    login.h \
    system/authentication.h

FORMS    += phonograph.ui \
    aboutdialog.ui \
    login.ui

RESOURCES += \
    icons.qrc \
    theme.qrc

TRANSLATIONS = phonograph_en.ts \
                   phonograph_gr.ts \
                   phonograph_dk.ts

OTHER_FILES +=
