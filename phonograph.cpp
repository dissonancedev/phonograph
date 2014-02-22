#include "phonograph.h"
#include "ui_phonograph.h"
#include <string>
#include <sstream>

/*** Constructor & destructor ***/

Phonograph::Phonograph(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Phonograph)
{
    // Setup the UI
    ui->setupUi(this);

    // Setup some CSS
    ui->mainframe->setStyleSheet( "QFrame#mainframe { background-image: url(:/images/background/theme/diskos-25-leivadia.jpg); }" );

    // Setup player and playlist
    this->playlist = new QMediaPlaylist;
    this->player = new QMediaPlayer(this, QMediaPlayer::StreamPlayback);
    this->player->setPlaylist( this->playlist );
    this->player->setVolume( this->ui->volume->value() );
    //player->setMedia(QUrl("http://rebetiko.sealabs.net/str.php?flok=08_-_Koula_fragosyriani.MP3"));
    //this->player->play();

    // Useful signals
    //connect(this->player, SIGNAL(positionChanged(qint64)), this, SLOT(setSliderPosition(qint64)));
    connect(this->player, SIGNAL(positionChanged(qint64)), this, SLOT(setPlaybackTimer(qint64)));
    connect(this->ui->seeker, SIGNAL(sliderMoved(int)), this, SLOT(setMediaPosition(int)));
    connect(this->player, SIGNAL(durationChanged(qint64)), this, SLOT(setMediaTime(qint64)));

    // Enable drag and drop for QListWidget and QTreeWidget
    /** TO-DO: Difficulties in implementing the drag & drop functionality **/
    this->ui->library->setDragEnabled(true);
    this->ui->playlist->setDragDropMode(QAbstractItemView::DropOnly);

    // Update library
    this->updateLibrary();

    // Load application settings
    this->loadSettings();
}

Phonograph::~Phonograph() {

    // Save settings when going out
    this->saveSettings();

    delete library;
    delete player;
    delete playlist;
    delete ui;
}

/*****************/
/*** Functions ***/
/*****************/

/**
 * @brief loadSettings loads all settings saved with QSettings
 */
void Phonograph::loadSettings() {

    // Initialize a QSettings object
    QSettings settings("RebetikoSealabs", "Phonograph");

    // Player settings
    this->ui->volume->setValue( settings.value( "player/volume" ).toInt() );
    this->ui->mute->setChecked( settings.value( "player/mute").toBool() );

}

/**
 * @brief saveSettings saves all the application's settings using QSettings
 */
void Phonograph::saveSettings() {

    // Initialize a QSettings object
    QSettings settings("RebetikoSealabs", "Phonograph");

    // Player settings
    settings.setValue( "player/volume" , this->ui->volume->value() );
    settings.setValue( "player/mute" , this->ui->mute->isChecked() );

}

/* Functions that sets the current playing media's current playback time */
void Phonograph::setPlaybackTimer(qint64 position) {

    // Calculate times
    int seconds = ( position / 1000 ) % 60;
    int minutes = round( ((position - seconds) / 1000) / 60 );

    // Set the label
    this->ui->startTimeLabel->setText( QString().sprintf("%02d", minutes) + QString(":") + QString().sprintf("%02d", seconds) );

    // Kata vasi den yparxei logos na mi setaristei ki auto edw. Kalytera to signal na einai syndedemeno me mia synartisi
    // Set new slider position, since we set it to tenths of seconds we can just apply directly
    if (!this->ui->seeker->isSliderDown()) {
        this->ui->seeker->setSliderPosition( round(position / 100) );
    }
}

/* Functions that sets the current playing media's total playback time */
void Phonograph::setMediaTime(qint64 duration) {

    // Calculate times
    int seconds = ( duration / 1000 ) % 60;
    int minutes = round( ((duration - seconds) / 1000) / 60 );

    // Dyo paratiriseis
    // 1. To if / else if / else den einai aparaitito giati paizoun synartisi pou formataroun arithmous me sygkekrimeno arithmo psifion.
    // Etsi mporeis na syriknwseis ton kwdika se mia grammi xwris if
    // 2. Xrhsimopoieis polu emeso tropo gia na setareis ena QString to opoio sto constructor tou dexetai diaforous typous kai tous metatrepei se string
    // Episis to QString sou parexei string concatenation me to + pragma pou alliws de ginetai sti C++
    // Set the label
    this->ui->endTimeLabel->setText( QString().sprintf("%02d", minutes) + QString(":") + QString().sprintf("%02d", seconds) );

    // Change the seeker range to achieve smoother scrolling. Set to tenths of seconds.
    this->ui->seeker->setRange(0, round(duration / 100) - 1);

    /*
    std::ostringstream stm_sec;
    std::ostringstream stm_min;
    stm_sec << seconds ;
    stm_min << minutes ;
    if (seconds>10 && minutes>10) {
        this->ui->endTimeLabel->setText(QString::fromStdString(stm_min.str() + ":" + stm_sec.str()));
    } else if (seconds>10 && minutes<10) {
        this->ui->endTimeLabel->setText(QString::fromStdString("0" + stm_min.str() + ":" + stm_sec.str()));
    } else if (seconds<10 && minutes>10) {
        this->ui->endTimeLabel->setText(QString::fromStdString(stm_min.str() + ":0" + stm_sec.str()));
    } else {
        this->ui->endTimeLabel->setText(QString::fromStdString("0" + stm_min.str() + ":0" + stm_sec.str()));
    }
    */
}

/* Function used as slot for enabling media seeking with QSlider */
void Phonograph::setMediaPosition(int position) {

    if (this->player->isSeekable()){
        this->player->setPosition( 100 * position);
    }

}

/**
 * Update function
*/
bool Phonograph::updateLibrary() {
    QString host("46.4.73.116");
    int port = 55432;
    QString dbname("phpbb2");

    // Get username and password from file
    QFile credentialsFile("credentials.txt");
    credentialsFile.open(QFile::ReadOnly); // Open file
    QTextStream fileStream(&credentialsFile); // Set stream
    QString line = fileStream.readAll().trimmed(); // Get the data trimmed
    QString username = line.split(",")[0];
    QString password = line.split(",")[1];
    // QString username( "" );
    // QString password( "" );

    // Create a music database object
    this->library = new MusicDatabase(host, port, username, password, dbname);

    // Try to update
    if (this->library->update()) {
        qDebug() << "Found " << this->library->songs.length() << " songs";
        // Add top level item
        QTreeWidgetItem *topLevel = new QTreeWidgetItem( this->ui->library );
        topLevel->setText(0, "Artists");
        topLevel->setIcon(0, QIcon(":/phonograph/general/icons/folder-sound.png"));

        int i;

        // Loop through the songs
        for (i = 0; i < this->library->songs.length(); i++) {

            this->addItemToLibrary( topLevel , this->library->songs[i] );

        }

        // Sort and expand
        this->ui->library->sortItems(0, Qt::AscendingOrder);
        this->ui->library->expandItem( topLevel );
    } else {
        qDebug() << this->library->getLastError();
    }

    return true;
}

void Phonograph::addItemToLibrary(QTreeWidgetItem *topLevel, Song song) {

    // Create the new item
    QSongItem *newSong = new QSongItem();
    newSong->setText(0, song.title);
    newSong->setIcon(0, QIcon(":/phonograph/general/icons/songbird.png"));
    newSong->song = song;

    int j;

    // Look if the same artist is there
    int found = -1;
    for (j = 0; j < topLevel->childCount(); j++) {
        if ( topLevel->child(j)->text(0) == song.composer) {
            found = j;
            break;
        }
    }

    QTreeWidgetItem *newArtist;
    if (found == -1) {

        // If artist not found there add it
        newArtist = new QTreeWidgetItem();
        newArtist->setText(0, song.composer);
        topLevel->addChild( newArtist );

        // Add icon to it
        newArtist->setIcon(0, QIcon(":/phonograph/general/icons/view-media-artist.png"));

        // Finally add the song to it
        newArtist->addChild( newSong );

    } else {

       newArtist = topLevel->child(j);

    }

    // Finally add the song to it
    newArtist->addChild( newSong );

}

void Phonograph::addItemToPlaylist(Song song) {
    // Create the item object
    QPlaylistItem *newItem = new QPlaylistItem();
    newItem->setText( song.composer + QString(" - ") + song.year + QString(" - ") + song.title );
    newItem->setIcon( QIcon(":/phonograph/general/icons/songbird.png") );
    newItem->song = song;

    // Put the item on the list and update media playlist
    this->ui->playlist->addItem( newItem );

    // And to the media playlist
    this->playlist->addMedia( QUrl(song.filename) );
}

/**
 * @brief Phonograph::updatePlaylist
 * This function updates the QMediaPlaylist object with the list's contents
 * Should be called everytime the playlist has been changed
 */
void Phonograph::updatePlaylist() {
    this->playlist->clear();

    int i;
    for (i = 0; i < this->ui->playlist->count(); i++) {
        QPlaylistItem *item = dynamic_cast<QPlaylistItem *>(this->ui->playlist->item(i));
        if (item) {
            this->playlist->addMedia( QUrl(item->song.filename) );
        }
    }

    this->player->setPlaylist(this->playlist);
}

void Phonograph::loadPlaylists() {

}

void Phonograph::loadPlaylist() {

}

void Phonograph::savePlaylist() {

}

/**************/
/*** Events ***/
/**************/

void Phonograph::on_library_itemDoubleClicked(QTreeWidgetItem *item, int column) {
    QSongItem *itemClicked = dynamic_cast<QSongItem *>(item);

    if (itemClicked) {
        this->addItemToPlaylist( itemClicked->song );
    }
}

void Phonograph::on_stop_clicked() {

    if ( this->player->state() == QMediaPlayer::PlayingState || this->player->state() == QMediaPlayer::PausedState) {
        this->player->stop();
        this->ui->play->setChecked( false );
    }

}

void Phonograph::on_volume_valueChanged(int value) {

    this->player->setVolume( value );

}

void Phonograph::on_mute_toggled(bool checked) {

    this->player->setMuted( checked );

}

void Phonograph::on_play_clicked(bool checked) {
    // Check if button is checked or not
    if (checked) {
        if (!this->playlist->isEmpty()) {

            int current = this->ui->playlist->currentRow();
            this->playlist->setCurrentIndex( current );
            this->player->play();

        }
    } else {

        if (this->player->state() == QMediaPlayer::PlayingState) {

            this->player->pause();

        }
    }
}

void Phonograph::on_playlist_itemDoubleClicked(QListWidgetItem *item) {

    if (!this->playlist->isEmpty()) {
        int current = this->ui->playlist->currentRow();
        this->playlist->setCurrentIndex( current );
        this->player->play();
    }

}

void Phonograph::on_skip_backward_clicked() {
    //player->setPlaylist(0);
    playlist->addMedia( QUrl("http://echidna-band.com/manifest/mp3/Manifests_Of_Human_Existence/08-Pendulum.mp3") );
    playlist->addMedia( QUrl::fromLocalFile("/home/verminoz/Music/giaf-giouf.mp3") );
    playlist->setCurrentIndex(0);

    this->player->play();
    qDebug() << "State: " << player->state();
    qDebug() << "Media State: " << player->mediaStatus();
    qDebug() << "Error: " << player->error() << " " << player->errorString();
    qDebug() << "Error: " << playlist->error() << " " << playlist->errorString();
}

void Phonograph::on_skip_forward_clicked() {

    this->playlist->next();

}

void Phonograph::on_clearPlaylist_clicked()
{
    this->ui->playlist->clear();
    updatePlaylist();
}

void Phonograph::on_addPlaylistItem_clicked()
{
    QList<QTreeWidgetItem *> selectedSongs = this->ui->library->selectedItems();

    int i;
    for(i = 0; i < selectedSongs.count(); i++){
        QSongItem *itemSelected = dynamic_cast<QSongItem *>(selectedSongs[i]);
        if (itemSelected) {
            this->addItemToPlaylist( itemSelected->song );
        }
    }
}

void Phonograph::on_removePlaylistItem_clicked()
{
    QList<QListWidgetItem *> selectedSongs = this->ui->playlist->selectedItems();

    if (!this->playlist->isEmpty()) {
        int i;
        for(i = 0; i < selectedSongs.count(); i++){
            QPlaylistItem *itemSelected = dynamic_cast<QPlaylistItem *>(selectedSongs[i]);
            int j;
            for(j = 0; j < this->ui->playlist->count(); j++){
                QPlaylistItem *playlistItem = dynamic_cast<QPlaylistItem *>(this->ui->playlist->item(j));
                if (itemSelected->song.id == playlistItem->song.id) {
                    delete this->ui->playlist->item(j);
                    updatePlaylist();
                }
            }
        }
    }

}
