#include "databasecache.h"

DatabaseCache::DatabaseCache() {

    // Set filename
    #ifdef Q_OS_WIN32
        filename =  QCoreApplication::applicationDirPath() + QString("\\dbcache.dat");
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

    if (!QFile::exists( filename )) {
        return;
    }

    file = new QFile( filename );
    file->open(QIODevice::ReadOnly);

    if (file->isOpen()) {
        stream.setDevice( file );

        int count;
        stream >> count;

        int i;
        Song tmp;
        songs.clear();
        for (i = 0; i < count; i++) {

            stream >> tmp.id;
            stream >> tmp.title;
            stream >> tmp.composer;
            stream >> tmp.performer1;
            stream >> tmp.year;
            stream >> tmp.description;

            songs.push_back( tmp );
        }

        file->close();
    }
}

void DatabaseCache::saveCache() {
    // Open stream
    file = new QFile( filename );

    // If file exists removed it first so that it is recreated
    if (file->exists()) {

        file->remove();

    }

    file->open(QIODevice::WriteOnly);

    if (file->isOpen()) {
        stream.setDevice( file );

        // Store number of songs
        stream << songs.count();

        int i;
        for (i = 0; i < songs.count(); i++) {

            stream << songs[i].id;
            stream << songs[i].title;
            stream << songs[i].composer;
            stream << songs[i].performer1;
            stream << songs[i].year;
            stream << songs[i].description;

        }

        file->close();
    }
}
