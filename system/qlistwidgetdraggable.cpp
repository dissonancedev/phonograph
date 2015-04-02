#include <system/qlistwidgetdraggable.h>

void QListWidgetDraggable::mouseMoveEvent(QMouseEvent * event) {

    if ( !(event->buttons() & Qt::LeftButton) ) {
        return;
    }

    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData();
    QString playlistName = this->selectedItems()[0]->text();

    // Collect data
    QByteArray data;
    QJsonDocument doc;
    QJsonObject obj;
    QJsonArray array;

    obj["playlist"] = playlistName;
    array.push_back(obj);

    // Put array in document
    doc.setArray(array);

    // Convert to JSON string
    data = doc.toJson();

    // Set the data
    mimeData->setData("application/json", data);
    drag->setMimeData(mimeData);
    drag->setPixmap( QPixmap(":/phonograph/general/icons/folder-multimedia.png") );

    // Start drag
    drag->exec( Qt::CopyAction );

}
