#ifndef DATABASECACHE_H
#define DATABASECACHE_H

#include <QDebug>
#include <QString>
#include <QFile>
#include "song.h"
#include <QDataStream>
#include <QCoreApplication>

class DatabaseCache
{
private:
    // Cache file information
    QString filename;
    QFile *file;
    QDataStream stream;

    // Song list
    QList<Song> songs;

    // Load cache function, runs with constructor
    void loadCache();

public:
    // Constructor
    DatabaseCache();

    // Constructor
    ~DatabaseCache();

    // Functions
    int count();
    QList<Song> getContent();
    void setContent(QList<Song> newContent);
    void saveCache();

};

#endif // DATABASECACHE_H
