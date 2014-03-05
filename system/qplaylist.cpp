#include "qplaylist.h"

QPlaylist::QPlaylist() {

}

QPlaylist::QPlaylist(QString name) {
    this->setName( name );

    QPlaylist();
}

// Getters
QString QPlaylist::getName() {

    return this->name;

}

QDateTime QPlaylist::getCreated() {

    return this->created;

}

QDateTime QPlaylist::getModified() {

    return this->modified;

}

QList<Song> QPlaylist::getPlaylist() {

    return this->playlist;

}

// Setters
void QPlaylist::setName(QString name) {

    if (!name.isEmpty()) {
        this->name = name;
    }

}

void QPlaylist::setPlayist(QList<Song> playlist) {

    this->playlist = playlist;

}

// I/O
void QPlaylist::load() {

    #ifdef Q_OS_WIN32
        QString filename = QCoreApplication::applicationDirPath() + QString("\playlists\\") + this->name + QString(".spl");
    #endif
    #ifdef Q_OS_LINUX
        QString filename = QCoreApplication::applicationDirPath() + QString("/playlists/") + this->name + QString(".spl");
    #endif

    // Check if playlist exists or not
    QFile file( filename );
    if (!file.exists()) {
        this->created = this->modified = QDateTime::currentDateTime();
    }
    this->modified = QDateTime::currentDateTime();

    // Open stream
    file.open(QIODevice::ReadOnly);

    if (file.isOpen()) {
        QDataStream stream( &file );

        // Read first the info
        // 1. name
        // 2. created
        // 3. modified
        stream >> this->name;
        stream >> this->created;
        stream >> this->modified;

        // Store 4. number of songs
        int count;
        stream >> count;

        int i;
        Song tmp;
        this->playlist.clear();
        for (i = 0; i < count; i++) {

            stream >> tmp.id;
            stream >> tmp.title;
            stream >> tmp.composer;
            stream >> tmp.performer1;

            this->playlist.push_back( tmp );
        }

        file.close();
    }

}

void QPlaylist::save() {

    #ifdef Q_OS_WIN32
        QString filename = QCoreApplication::applicationDirPath() + QString("\playlists\\") + this->name + QString(".spl");
    #endif
    #ifdef Q_OS_LINUX
        QString filename = QCoreApplication::applicationDirPath() + QString("/playlists/") + this->name + QString(".spl");
    #endif

    // Check if playlist exists or not
    QFile file( filename );
    if (!file.exists()) {
        this->created = this->modified = QDateTime::currentDateTime();
    }
    this->modified = QDateTime::currentDateTime();

    // Open stream
    file.open(QIODevice::WriteOnly);

    if (file.isOpen()) {
        QDataStream stream( &file );

        // Store first the info
        // 1. name
        // 2. created
        // 3. modified
        stream << this->name << this->created << this->modified;

        // Store 4. number of songs
        stream << this->playlist.count();

        int i;
        for (i = 0; i < this->playlist.count(); i++) {

            stream << playlist[i].id;
            stream << playlist[i].title;
            stream << playlist[i].composer;
            stream << playlist[i].performer1;

        }

        file.close();
    }

}
