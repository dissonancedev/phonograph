#include "qplaylistwidget.h"

QPlaylistWidget::QPlaylistWidget(): QListWidget() {

}

QPlaylistWidget::QPlaylistWidget(QWidget *parent): QListWidget(parent) {


}

void QPlaylistWidget::dropEvent ( QDropEvent * event ) {


    qDebug() << "skata";
    if (event->mimeData()->hasText()) {

        qDebug() << event->mimeData()->text();

    }

}
