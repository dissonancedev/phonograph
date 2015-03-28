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
