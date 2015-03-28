#include "playlistnamedialog.h"
#include "ui_playlistnamedialog.h"

PlaylistNameDialog::PlaylistNameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlaylistNameDialog)
{
    ui->setupUi(this);
}

PlaylistNameDialog::~PlaylistNameDialog()
{
    delete ui;
}

void PlaylistNameDialog::on_buttonBox_accepted()
{
    QString name = this->ui->playlistNameEdit->text();

    // Playlist handler
    QPlaylist newPlaylist;
    newPlaylist.setName(name);
    newPlaylist.save();
    emit loadPlaylists();
}
