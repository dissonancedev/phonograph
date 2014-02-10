#ifndef SONG_H
#define SONG_H

#include <QString>

class Song {
public:
    int id;
    QString title;
    QString composer;
    QString performer1;
    QString performer2;
    QString year;
    QString info;
    QString filename;
};

#endif // SONG_H
