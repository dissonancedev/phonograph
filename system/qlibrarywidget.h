#ifndef QLIBRARYWIDGET_H
#define QLIBRARYWIDGET_H

#include <QDebug>

#include <QTreeWidget>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include "qsongitem.h"

class QLibraryWidget: public QTreeWidget
{
public:
    QLibraryWidget(): QTreeWidget() {
        this->setDragEnabled( true );
    }
    QLibraryWidget(QWidget *parent = 0): QTreeWidget(parent) {}

protected:
    void mouseMoveEvent(QMouseEvent * event);
};

#endif // QLIBRARYWIDGET_H
