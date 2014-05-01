#include "qlibrarywidget.h"

void QLibraryWidget::mouseMoveEvent(QMouseEvent * event) {

    if ( !(event->buttons() & Qt::LeftButton) ) {
        return;
    }

    // Create drag and mime data objects
    QDrag *drag = new QDrag( this );
    QMimeData *mimeData = new QMimeData();

    // Collect data
    QJsonObject obj;
    QByteArray data;
    QJsonDocument doc;
    for (int i = 0; i < this->selectedItems().count(); i++) {

        // See if item can be cast to QSongItem
        QSongItem *songItem = dynamic_cast<QSongItem *>(this->selectedItems()[i]);

        if (songItem) {

            // Convert to JSON object
            obj["composer"] = songItem->song.composer;
            obj["filename"] = songItem->song.filename;
            obj["id"] = songItem->song.id;
            obj["performer1"] = songItem->song.performer1;
            obj["title"] = songItem->song.title;

            // Convert to JSON string
            doc.setObject(obj);
            data += doc.toJson();

        } else {

            for (int j = 0; j < this->selectedItems()[i]->childCount(); j++) {

                QSongItem *songItem = dynamic_cast<QSongItem *>(this->selectedItems()[i]->child(j));

                // Convert to JSON object
                obj["composer"] = songItem->song.composer;
                obj["filename"] = songItem->song.filename;
                obj["id"] = songItem->song.id;
                obj["performer1"] = songItem->song.performer1;
                obj["title"] = songItem->song.title;

                // Convert to JSON string
                doc.setObject(obj);
                data += doc.toJson();

            }

        }

    }

    // Set the data
    mimeData->setData("application/json", data);
    drag->setMimeData(mimeData);
    drag->setPixmap( QPixmap(":/phonograph/general/icons/songbird.png") );

qDebug() << drag->supportedActions().testFlag( Qt::CopyAction );
qDebug() << drag->supportedActions().testFlag( Qt::MoveAction );
qDebug() << drag->supportedActions().testFlag( Qt::LinkAction );
qDebug() << drag->supportedActions().testFlag( Qt::ActionMask );
qDebug() << drag->supportedActions().testFlag( Qt::IgnoreAction );
qDebug() << drag->supportedActions().testFlag( Qt::TargetMoveAction );

    Qt::DropAction dropAction = drag->exec( Qt::CopyAction );

}
