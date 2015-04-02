#include "playlistedit.h"
#include "ui_playlistedit.h"

PlaylistEditDialog::PlaylistEditDialog(QWidget *parent, QListWidgetItem *item) :
    QDialog(parent),
    ui(new Ui::PlaylistEditDialog)
{
    ui->setupUi(this);
    this->item = item;

    //this->ui->playlistName->setText( item->text() );
    this->selectedPlaylist.setName(item->text());
    this->selectedPlaylist.load();
    QList<Song> contents = this->selectedPlaylist.getPlaylist();

    for (int i = 0; i < contents.size(); i++) {

        // Create the item object
        QPlaylistItem *newItem = new QPlaylistItem();
        newItem->setText( contents[i].composer + QString(" - ") + contents[i].performer1 + QString(" - ") + contents[i].title );
        newItem->setIcon( QIcon(":/phonograph/general/icons/songbird.png") );
        newItem->song = contents[i];

        this->ui->playlistSongsListWidget->addItem(newItem);
    }
}

PlaylistEditDialog::~PlaylistEditDialog()
{
    delete ui;
}

void PlaylistEditDialog::on_removeSong_clicked()
{
    if (this->ui->playlistSongsListWidget->selectedItems().count() > 0) {
        this->selectedPlaylist.removeItem(this->ui->playlistSongsListWidget->selectedItems()[0]->text());
        qDeleteAll(this->ui->playlistSongsListWidget->selectedItems());
    }
}

void PlaylistEditDialog::on_buttonBox_accepted()
{
    this->selectedPlaylist.save();
}

void PlaylistEditDialog::on_addToCurrent_clicked()
{
    // Get pointer to main window in order to call functions from that class
    Phonograph *mainWindow = (Phonograph *)this->getParentWindow();

    // Get the selected items
    QList<QListWidgetItem *> selectedSongs = this->ui->playlistSongsListWidget->selectedItems();

    // Loop through them and extract Song objects
    QList<Song> allSongs;
    for (int i = 0; i < selectedSongs.count(); i++) {

        QPlaylistItem *item = dynamic_cast<QPlaylistItem *> (selectedSongs[i]);
        allSongs.push_back( item->song );

    }

    // If there where songs then call addItemsToPlaylist to add them
    if (allSongs.size() > 0) {
        mainWindow->addItemsToPlaylist(allSongs);
    }
}

/**
 * @brief QPlaylistWidget::getParentWindow
 * @return The widget's parent window
 */
QWidget* PlaylistEditDialog::getParentWindow() {

    // Loop until we reach the top level wiget
    QWidget *widget = this;
    while (widget->parentWidget()) {
        widget = widget->parentWidget();
    }

    return widget;

}

void PlaylistEditDialog::on_addToList_clicked()
{
    // Get pointer to main window in order to call functions from that class
    Phonograph *mainWindow = (Phonograph *)this->getParentWindow();
    QList<QListWidgetItem *> selectedItems = this->ui->playlistSongsListWidget->selectedItems();
    if (selectedItems.count() > 0) {
        selectedSong = selectedItems[0];
        mainWindow->addToExistingPlaylist("playlist");
    }
}
