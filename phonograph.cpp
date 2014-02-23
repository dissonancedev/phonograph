#include "phonograph.h"
#include "ui_phonograph.h"
#include <string>
#include <sstream>
#include <QGraphicsDropShadowEffect>
#include <QStyleFactory>

/*** Constructor & destructor ***/

Phonograph::Phonograph(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Phonograph)
{
    QStyleFactory styleFactory;
    this->setStyle(styleFactory.create("plastiq ue"));

    // Setup the UI
    ui->setupUi(this);

    // Setup some CSS
    ui->mainframe->setStyleSheet( "QFrame#mainframe { background-image: url(:/images/background/theme/diskos-25-leivadia.jpg); }" );

    // Setup player and playlist
    this->playlist = new QMediaPlaylist;
    this->player = new QMediaPlayer(this, QMediaPlayer::StreamPlayback);
    this->player->setPlaylist( this->playlist );
    this->player->setVolume( this->ui->volume->value() );
    //QString ss = "http://rebetiko.sealabs.net/str.php?flok=Î\221Î Î\225Î\233Î Î\231Î£Î\234Î\225Î\235Î\237Î£(Î¡Î\231";
    //player->setMedia(QUrl(QString::from(ss.toStdString().c_str())));
    //this->player->play();
    //QString url("http://rebetiko.sealabs.net/str.php?flok=ΓΑΛΑΝΟΜΑΤΑ-.mp3");
    //player->setMedia(QUrl::fromAce(url));
    //this->player->play();

    // Useful signals
    connect(this->player, SIGNAL(currentMediaChanged(QMediaContent)), this, SLOT(setPlayingSongLabel(QMediaContent)));
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

    // Add shadow effects to several GUI elements for better visualization
    QGraphicsDropShadowEffect *nowPLayingEffect = new QGraphicsDropShadowEffect(this);
    QGraphicsDropShadowEffect *categorizeByLabelEffect = new QGraphicsDropShadowEffect(this);
    QGraphicsDropShadowEffect *volumeLabelEffect = new QGraphicsDropShadowEffect(this);
    QGraphicsDropShadowEffect *startTimeLabelEffect = new QGraphicsDropShadowEffect(this);
    QGraphicsDropShadowEffect *endTimeLabelEffect = new QGraphicsDropShadowEffect(this);

    nowPLayingEffect->setBlurRadius(0);
    nowPLayingEffect->setColor(QColor("#000000"));
    nowPLayingEffect->setOffset(2,2);

    categorizeByLabelEffect->setBlurRadius(0);
    categorizeByLabelEffect->setColor(QColor("#000000"));
    categorizeByLabelEffect->setOffset(1,1);

    volumeLabelEffect->setBlurRadius(0);
    volumeLabelEffect->setColor(QColor("#000000"));
    volumeLabelEffect->setOffset(1,1);

    startTimeLabelEffect->setBlurRadius(0);
    startTimeLabelEffect->setColor(QColor("#000000"));
    startTimeLabelEffect->setOffset(1,1);

    endTimeLabelEffect->setBlurRadius(0);
    endTimeLabelEffect->setColor(QColor("#000000"));
    endTimeLabelEffect->setOffset(1,1);

    this->ui->playingNowLabel->setGraphicsEffect(nowPLayingEffect);
    this->ui->categorizeByLabel->setGraphicsEffect(categorizeByLabelEffect);
    this->ui->volume_label->setGraphicsEffect(volumeLabelEffect);
    this->ui->startTimeLabel->setGraphicsEffect(startTimeLabelEffect);
    this->ui->endTimeLabel->setGraphicsEffect(endTimeLabelEffect);

    this->ui->libraryTabWidget->setStyleSheet("QTabWidget,QTabWidget::pane,QTabBar { background-color: rgb(117, 117, 117, 200); border: 0px; }");
    this->ui->tabWidget->setStyleSheet("QTabWidget,QTabWidget::pane,QTabBar { background-color: rgb(117, 117, 117, 200); border: 0px; }");

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
        this->player->setPosition(100 * position);
    }

}

/* Slot that receives a signal when the media changes, so that several ui elements are updated */
void Phonograph::setPlayingSongLabel(QMediaContent content){
    if (content.isNull() == false){
        QUrl url = content.canonicalUrl();
        int i = this->playlist->mediaCount();
        int j;
        for(j = 0; j < i; j++){
            if(url == this->playlist->media(j).canonicalUrl()){
                break;
            }
        }

        QPlaylistItem *currItem = (QPlaylistItem*)this->ui->playlist->item(j);
        this->ui->playingNowLabel->setText(currItem->song.composer + " - " + currItem->song.title);
        this->ui->playingNowLabel->setAlignment(Qt::AlignCenter);
        QFont font("MS Shell Dlg 2", 12, QFont::Bold);
        this->ui->playingNowLabel->setFont(font);
        this->ui->playlist->setCurrentRow(j);
    }
    else { //the playlist has finished
        this->ui->play->setChecked(false);
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

    //If the song does not exists in the current playlist...
    bool exists = false;
    for(int i = 0; i < this->ui->playlist->count(); i++){
        QPlaylistItem *tmp = dynamic_cast<QPlaylistItem *>(this->ui->playlist->item(i));
        if (newItem->song.filename == tmp->song.filename) exists = true;
    }

    if(!exists){
        // ...put the item on the list and update media playlist...
        this->ui->playlist->addItem( newItem );
        // ...and to the media playlist
        QString url = normalizeUrl(song.filename);
        this->playlist->addMedia( QUrl(url) );
    }
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
            QString url = normalizeUrl(item->song.filename);
            this->playlist->addMedia( QUrl(url) );
        }
    }

    this->player->setPlaylist(this->playlist);
}

/* Function that encodes the greek filenames to the proper format */
QString Phonograph::normalizeUrl(QString url) {
    url.replace("Α", "%C1");
    url.replace("Β", "%C2");
    url.replace("Γ", "%C3");
    url.replace("Δ", "%C4");
    url.replace("Ε", "%C5");
    url.replace("Ζ", "%C6");
    url.replace("Η", "%C7");
    url.replace("Θ", "%C8");
    url.replace("Ι", "%C9");
    url.replace("Κ", "%CA");
    url.replace("Λ", "%CB");
    url.replace("Μ", "%CC");
    url.replace("Ν", "%CD");
    url.replace("Ξ", "%CE");
    url.replace("Ο", "%CF");
    url.replace("Π", "%D0");
    url.replace("Ρ", "%D1");
    url.replace("Ό", "%D2");
    url.replace("Σ", "%D3");
    url.replace("Τ", "%D4");
    url.replace("Υ", "%D5");
    url.replace("Φ", "%D6");
    url.replace("Χ", "%D7");
    url.replace("Ψ", "%D8");
    url.replace("Ω", "%D9");

    url.replace("Ϊ",  "%DA");
    url.replace("Ϋ",  "%DB");
    url.replace("ά",  "%DC");
    url.replace("έ",  "%DD");
    url.replace("ή",  "%DE");
    url.replace("ί",  "%DF");
    url.replace("ΰ",  "%E0");
    url.replace("α",  "%E1");
    url.replace("β",  "%E2");
    url.replace("γ",  "%E3");
    url.replace("δ",  "%E4");
    url.replace("ε",  "%E5");
    url.replace("ζ",  "%E6");
    url.replace("η",  "%E7");
    url.replace("θ",  "%E8");
    url.replace("ι",  "%E9");
    url.replace("κ",  "%EA");
    url.replace("λ",  "%EB");
    url.replace("μ",  "%EC");
    url.replace("ν",  "%ED");
    url.replace("ξ",  "%EE");
    url.replace("ο",  "%EF");
    url.replace("π",  "%F0");
    url.replace("ρ",  "%F1");
    url.replace("ς",  "%F2");
    url.replace("σ",  "%F3");
    url.replace("τ",  "%F4");
    url.replace("υ",  "%F5");
    url.replace("φ",  "%F6");
    url.replace("χ",  "%F7");
    url.replace("ψ",  "%F8");
    url.replace("ω",  "%F9");
    url.replace("ϊ",  "%FA");
    url.replace("ϋ",  "%FB");
    url.replace("ό",  "%FC");
    url.replace("ύ",  "%FD");
    url.replace("ώ",  "%FE");
    url.replace("Ά",  "%B6");
    url.replace("Έ",  "%B8");
    url.replace("Ή",  "%B9");
    url.replace("Ί",  "%BA");
    url.replace("Ό",  "%BC");
    url.replace("Ύ",  "%BE");
    url.replace("Ώ",  "%BF");

    url.replace("'", "%5C%27");

    return url;
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

        if (this->player->state() == QMediaPlayer::PausedState) {
            this->player->play();
        }

        else if (!this->playlist->isEmpty()) {
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
        this->ui->play->setChecked(true);
    }

}

void Phonograph::on_skip_backward_clicked() {
    if(this->playlist->previousIndex() != -1)
        this->playlist->previous();

}

void Phonograph::on_skip_forward_clicked() {
    if(this->playlist->nextIndex() != -1)
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

void Phonograph::on_shuffle_clicked(bool checked)
{
    if(checked) {
        this->ui->toolButton->setChecked(false);
        this->playlist->shuffle();
    }
    else this->playlist->setPlaybackMode(QMediaPlaylist::Sequential);
}

void Phonograph::on_toolButton_clicked(bool checked)
{
    if(checked) {
        this->ui->shuffle->setChecked(false);
        this->playlist->setPlaybackMode(QMediaPlaylist::Loop);
    }
    else this->playlist->setPlaybackMode(QMediaPlaylist::Sequential);
}

void Phonograph::on_seek_forward_clicked()
{
    int pos = this->ui->seeker->sliderPosition();
    setMediaPosition(pos+100);
}

void Phonograph::on_seek_backward_clicked()
{
    int pos = this->ui->seeker->sliderPosition();
    setMediaPosition(pos-100);
}
