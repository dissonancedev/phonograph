#include "databasecache.h"

DatabaseCache::DatabaseCache() {

    // Set filename
    #ifdef Q_OS_WIN32
        filename =  QCoreApplication::applicationDirPath() + QString("\dbcache.dat");
    #endif
    #ifdef Q_OS_LINUX
        filename =  QCoreApplication::applicationDirPath() + QString("/dbcache.dat");
    #endif

    // Load the data from the file
    this->loadCache();

}

DatabaseCache::~DatabaseCache() {

    // Destroy file
    if (this->file) {
        if (this->file->isOpen()) {
            this->file->close();
        }

        delete this->file;
    }

}

int DatabaseCache::count() {
    return this->songs.count();
}

QList<Song> DatabaseCache::getContent() {
    return this->songs;
}

void DatabaseCache::setContent(QList<Song> newContent) {
    this->songs = newContent;
}

void DatabaseCache::loadCache() {
    file = new QFile( filename );
    file->open(QIODevice::ReadOnly);

    if (file->isOpen()) {
        stream.setDevice( file );

        int count;
        stream >> count;

        int i;
        Song tmp;
        for (i = 0; i < count; i++) {
            stream >> tmp.composer;
            stream >> tmp.filename;
            stream >> tmp.id;
            stream >> tmp.info;
            stream >> tmp.performer1;
            stream >> tmp.performer2;
            stream >> tmp.title;
            stream >> tmp.year;
            songs.push_back( tmp );
        }

        file->close();
    }
}

void DatabaseCache::saveCache() {
    // Open stream
    file = new QFile( filename );
    file->open(QIODevice::WriteOnly);

    if (file->isOpen()) {
        stream.setDevice( file );

        // Store number of songs
        stream << songs.count();

        int i;
        for (i = 0; i < songs.count(); i++) {
            stream << songs[i].composer;
            stream << songs[i].filename;
            stream << songs[i].id;
            stream << songs[i].info;
            stream << songs[i].performer1;
            stream << songs[i].performer2;
            stream << songs[i].title;
            stream << songs[i].year;
        }

        file->close();
    }
}
