#ifndef QPLAYLISTWIDGET_H
#define QPLAYLISTWIDGET_H

#include <QDebug>

#include <QListWidget>
#include <QDropEvent>
#include <QMimeData>

class QPlaylistWidget: public QListWidget
{
public:
    QPlaylistWidget(): QListWidget() {

        this->setAcceptDrops( true );
        this->setDragDropMode( QAbstractItemView::DragDrop );
        this->viewport()->setAcceptDrops( true );
        this->setDropIndicatorShown( true );

    }
    QPlaylistWidget(QWidget *parent = 0): QListWidget(parent) {}
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent * event);
};

#endif // QPLAYLISTWIDGET_H
