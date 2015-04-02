#ifndef QLISTWIDGETDRAGGABLE_H
#define QLISTWIDGETDRAGGABLE_H

#include <QListWidget>
#include <QMimeData>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMouseEvent>
#include <QDrag>

class QListWidgetDraggable: public QListWidget
{
public:
    QListWidgetDraggable(): QListWidget() {
        this->setDragEnabled( true );
    }
    QListWidgetDraggable(QWidget *parent = 0): QListWidget(parent) {}

protected:
    void mouseMoveEvent(QMouseEvent * event);
};

#endif // QLISTWIDGETDRAGGABLE_H

