#ifndef QPLAYLISTITEM_H
#define QPLAYLISTITEM_H

#include "song.h"
#include <QListWidgetItem>

class QPlaylistItem: public QListWidgetItem
{
public:
    Song song;

    QPlaylistItem(): QListWidgetItem() {}
};

#endif // QPLAYLISTITEM_H
