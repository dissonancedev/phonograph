#ifndef QSONGITEM_H
#define QSONGITEM_H

#include <QTreeWidgetItem>
#include "song.h"

class QSongItem: public QTreeWidgetItem
{


public:
    Song song;

    QSongItem();
};

#endif // QSONGITEM_H
