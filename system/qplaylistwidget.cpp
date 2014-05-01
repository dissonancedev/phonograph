#include "qplaylistwidget.h"

void QPlaylistWidget::dragEnterEvent(QDragEnterEvent *event) {

  if (event->mimeData()->hasFormat("application/json")) {

        event->acceptProposedAction();

  }

}

void QPlaylistWidget::dragMoveEvent(QDragMoveEvent *event) {

  if (event->mimeData()->hasFormat("application/json")) {

        event->acceptProposedAction();

  }

}

void QPlaylistWidget::dropEvent(QDropEvent * event) {


    QString data = event->mimeData()->data("application/json");

}
