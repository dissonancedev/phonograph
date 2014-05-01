#include "qplaylistwidget.h"

void QPlaylistWidget::dragEnterEvent(QDragEnterEvent *event) {

  //  if (event->mimeData()->hasFormat("application/json")) {
        event->acceptProposedAction();
   // }

}

void QPlaylistWidget::dropEvent(QDropEvent * event) {

    qDebug() << event->mimeData()->data("text/plain");

    //event->acceptProposedAction();

}
