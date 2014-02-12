#include "phonograph.h"
#include "ui_phonograph.h"

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


    this->player->setNotifyInterval(100);
    connect(this->player, SIGNAL(positionChanged(qint64)), this, SLOT(setSliderPosition(qint64)));
    connect(this->ui->seeker, SIGNAL(sliderMoved(int)), this, SLOT(setMediaPosition(int)));
    // Update library
    this->updateLibrary();
}

Phonograph::~Phonograph()
{
    delete library;
    delete player;
    delete playlist;
    delete ui;
}

/*****************/
/*** Functions ***/
/*****************/

/* Function used as slot for enabling media seeking with QSlider */
void Phonograph::setMediaPosition(int position) {

    if(this->player->isSeekable()){
        double pos = (position/100.0) * (this->player->duration()+0.0000000001);
        this->player->setPosition(pos);
    }

}

/* Function used as slot for setting QSlider's position according to the
   song being played */
void Phonograph::setSliderPosition(qint64 position) {

    this->ui->seeker->setSliderPosition((position/( this->player->duration()+0.0000000001))*100);
    if (((position/( this->player->duration()+0.0000000001))*100) >= 99) this->ui->seeker->setSliderPosition(100); // song finished

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
    updatePlaylist();
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

/************/
/*** Events ***/
/************/

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

void Phonograph::on_skip_backward_clicked() {
    player->setPlaylist(0);
    player->setMedia(QUrl("http://echidna-band.com/manifest/mp3/Manifests_Of_Human_Existence/08-Pendulum.mp3"));
    //player->setMedia(QUrl::fromLocalFile("/home/verminoz/Music/giaf-giouf.mp3"));
    this->player->play();
    qDebug() << "State: " << player->state();
    qDebug() << "Media State: " << player->mediaStatus();
    qDebug() << "Error: " << player->error() << " " << player->errorString();
}
