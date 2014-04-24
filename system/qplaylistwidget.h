#ifndef QPLAYLISTWIDGET_H
#define QPLAYLISTWIDGET_H

#include <QDebug>

#include <QListWidget>
#include <QDropEvent>
#include <QMimeData>

class QPlaylistWidget: public QListWidget
{
public:
    QPlaylistWidget();
    QPlaylistWidget(QWidget *parent);
protected:
    void dropEvent ( QDropEvent * event );
};

#endif // QPLAYLISTWIDGET_H
