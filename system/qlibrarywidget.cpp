#include "qlibrarywidget.h"

void QLibraryWidget::mouseMoveEvent(QMouseEvent * event) {

    if ( !(event->buttons() & Qt::LeftButton) ) {
        return;
    }

    // Create drag and mime data objects
    QDrag *drag = new QDrag( this );
    QMimeData *mimeData = new QMimeData();

    // Collect data
    QByteArray data;
    QJsonDocument doc;
    QJsonObject obj;
    QJsonArray array;
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
            obj["year"] = songItem->song.year;

            // Push in the array
            array.push_back( obj );

        } else {

            for (int j = 0; j < this->selectedItems()[i]->childCount(); j++) {

                QSongItem *songItem = dynamic_cast<QSongItem *>(this->selectedItems()[i]->child(j));

                // Convert to JSON object
                obj["composer"] = songItem->song.composer;
                obj["filename"] = songItem->song.filename;
                obj["id"] = songItem->song.id;
                obj["performer1"] = songItem->song.performer1;
                obj["title"] = songItem->song.title;
                obj["year"] = songItem->song.year;

                // Push in the array
                array.push_back( obj );

            }

        }

    }

    // Put array in document
    doc.setArray( array );

    // Convert to JSON string
    data = doc.toJson();

    // Set the data
    mimeData->setData("application/json", data);
    drag->setMimeData(mimeData);
    drag->setPixmap( QPixmap(":/phonograph/general/icons/songbird.png") );

    // Start drag
    drag->exec( Qt::CopyAction );

}
