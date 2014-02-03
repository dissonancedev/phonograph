#ifndef SONG_H
#define SONG_H

#include <QString>
#include <QAbstractItemModel>

class Song: public QAbstractItemModel {
public:
    QString title;
    QString artist;
    QString year;
};

#endif // SONG_H
