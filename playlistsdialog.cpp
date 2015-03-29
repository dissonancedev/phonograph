#include "playlistsdialog.h"
#include "ui_playlistsdialog.h"

PlaylistsDialog::PlaylistsDialog(QWidget *parent, QString isFromLibrary, QLibraryWidget *library, QListWidget *playlist) :
    QDialog(parent),
    ui(new Ui::PlaylistsDialog)
{
    ui->setupUi(this);
    this->library = library;
    this->playlist = playlist;
    this->isFromLibrary = isFromLibrary;
     this->ui->dialogPlaylistsList->clear();
    QDir directory(QCoreApplication::applicationDirPath() + QString("/playlists"));
    QStringList files = directory.entryList( QStringList("*.spl") );

    for (int i = 0; i < files.count(); i++) {

        QListWidgetItem *newItem = new QListWidgetItem();
        newItem->setText( files[i].left( files[i].indexOf(".") ) );
        newItem->setIcon( QIcon(":/phonograph/general/icons/folder-multimedia.png") );
        this->ui->dialogPlaylistsList->addItem( newItem );

    }
}

PlaylistsDialog::~PlaylistsDialog()
{
    delete ui;
}

void PlaylistsDialog::on_buttonBox_accepted()
{
    QList<QListWidgetItem *> selectedPlaylists  = this->ui->dialogPlaylistsList->selectedItems();

    if (selectedPlaylists.length() > 0) {
        QList< Song > allSongs;
        QPlaylist *playlistSelected = new QPlaylist();
        // Create a playlist object
        playlistSelected->setName(selectedPlaylists[0]->text());

        // Load the playlist
        playlistSelected->load();
        QList<Song> contents = playlistSelected->getPlaylist();

        if (isFromLibrary == "true") {

            QList<QTreeWidgetItem *> selectedItems = library->selectedItems();

            // Loop through them and extract Song objects

            for (int i = 0; i < selectedItems.count(); i++) {

                // Cast to QSongItem to get Song object
                QSongItem *item = dynamic_cast<QSongItem *>(selectedItems[i]);
                if (item) {

                    // Put it in list
                    allSongs.push_back( item->song );

                } else {

                    for (int j = 0; j < selectedItems[i]->childCount(); j++) {

                        QSongItem *childItem = dynamic_cast<QSongItem *>(selectedItems[i]->child(j));
                        if (childItem) {

                            // Put it in list
                            allSongs.push_back( childItem->song );

                        }
                    }
                }
            }
        }

        else {

            QList<QListWidgetItem *> selectedItems = playlist->selectedItems();
            for (int i = 0; i < selectedItems.count(); i++) {

                QPlaylistItem *item = dynamic_cast<QPlaylistItem *> (selectedItems[i]);
                allSongs.push_back( item->song );

            }
        }

        // If there where songs then call addItemsToPlaylist to add them
        if (allSongs.size() > 0) {
            contents.append(allSongs);
        }

        playlistSelected->setPlayist(contents);
        playlistSelected->save();
    }

}
