#ifndef SONG_H
#define SONG_H

#include <QString>

class Song {
public:
    int id;
    QString title;
    QString composer;
    QString performer1;
    QString filename;
    static QString base_filename;
};

#endif // SONG_H
