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

    // Setup the flags
    this->isDialogShown = false;
    this->wasMinimized = false;

    /** Setup some CSS **/
    ui->mainframe->setStyleSheet( "QFrame#mainframe { border-image: url(:/images/background/theme/diskos-25-leivadia.jpg); }" );
    ui->playerFrame->setStyleSheet("QFrame#playerFrame { background-color: qconicalgradient(cx:1, cy:1, angle:286.6, stop:0.170455 rgba(0, 0, 0, 185),"
                                   "stop:0.806818 rgba(236, 236, 236, 191)); }");

    // Add shadow effects to several GUI elements for better visualization
    QGraphicsDropShadowEffect *nowPLayingEffect = new QGraphicsDropShadowEffect(this);
    QGraphicsDropShadowEffect *categorizeByLabelEffect = new QGraphicsDropShadowEffect(this);
    QGraphicsDropShadowEffect *volumeLabelEffect = new QGraphicsDropShadowEffect(this);
    QGraphicsDropShadowEffect *startTimeLabelEffect = new QGraphicsDropShadowEffect(this);
    QGraphicsDropShadowEffect *endTimeLabelEffect = new QGraphicsDropShadowEffect(this);

    nowPLayingEffect->setBlurRadius(1);
    nowPLayingEffect->setColor(QColor("#000000"));
    nowPLayingEffect->setOffset(1,1);

    categorizeByLabelEffect->setBlurRadius(1);
    categorizeByLabelEffect->setColor(QColor("#000000"));
    categorizeByLabelEffect->setOffset(1,1);

    volumeLabelEffect->setBlurRadius(1);
    volumeLabelEffect->setColor(QColor("#000000"));
    volumeLabelEffect->setOffset(1,1);

    startTimeLabelEffect->setBlurRadius(1);
    startTimeLabelEffect->setColor(QColor("#000000"));
    startTimeLabelEffect->setOffset(1,1);

    endTimeLabelEffect->setBlurRadius(1);
    endTimeLabelEffect->setColor(QColor("#000000"));
    endTimeLabelEffect->setOffset(1,1);

    this->ui->playingNowLabel->setGraphicsEffect(nowPLayingEffect);
    this->ui->categorizeByLabel->setGraphicsEffect(categorizeByLabelEffect);
    this->ui->volume_label->setGraphicsEffect(volumeLabelEffect);
    this->ui->startTimeLabel->setGraphicsEffect(startTimeLabelEffect);
    this->ui->endTimeLabel->setGraphicsEffect(endTimeLabelEffect);

    this->ui->libraryTabWidget->setStyleSheet("QTabWidget,QTabWidget::pane,QTabBar { background-color: rgb(117, 117, 117, 200); border: 0px; }");
    this->ui->tabWidget->setStyleSheet("QTabWidget,QTabWidget::pane,QTabBar { background-color: rgb(117, 117, 117, 200); border: 0px; }");

    /** --------------------- **/

    // Setup system tray
    this->createTrayIcon();
    this->spawnTrayMenu();

    // Setup player and playlist
    this->playlist = new QMediaPlaylist;
    this->player = new QMediaPlayer(this, QMediaPlayer::StreamPlayback);
    this->player->setPlaylist( this->playlist );
    this->player->setVolume( this->ui->volume->value() );

    // Useful signals
    connect(this->player, SIGNAL(currentMediaChanged(QMediaContent)), this, SLOT(setPlayingSongLabel(QMediaContent)));
    connect(this->player, SIGNAL(positionChanged(qint64)), this, SLOT(setPlaybackTimer(qint64)));
    connect(this->ui->seeker, SIGNAL(sliderMoved(int)), this, SLOT(setMediaPosition(int)));
    connect(this->player, SIGNAL(durationChanged(qint64)), this, SLOT(setMediaTime(qint64)));

    // Playlist popup
    this->createPlaylistPopup();

    // Enable drag and drop for QListWidget and QTreeWidget
    /** TO-DO: Difficulties in implementing the drag & drop functionality **/
    this->ui->library->setDragEnabled(true);
    this->ui->playlist->setDragDropMode(QAbstractItemView::DragDrop);

}

void Phonograph::showEvent(QShowEvent *event) {

    // Check flag to avoid execution during restoring from system tray
    if (!this->wasMinimized) {

        QMainWindow::showEvent(event);
        QTimer::singleShot(0, this, SLOT(afterShowEvent()));

    }

}

void Phonograph::afterShowEvent() {

        // Load application settings
        this->loadSettings();

        // Update library
        updateLibrary();

        // Load the user's playlists
        this->loadPlaylists();

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
    //this->ui->categorizeBySelect->setCurrentIndex( settings.value( "library/categorizeby").toInt() );

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
    settings.setValue( "library/categorizeby" , this->ui->categorizeBySelect->currentIndex() );

}

void Phonograph::createPlaylistPopup() {

    // Create actions
    QAction* libraryAction1 = new QAction("Delete playlist", ui->savedPlaylists);

    // Connect the signals
    connect(libraryAction1, SIGNAL(triggered()), this, SLOT(deletePlaylist()));

    // Add the action
    this->ui->savedPlaylists->addAction(libraryAction1);

    // Configure the list widget to display actions as popup menu
    this->ui->savedPlaylists->setContextMenuPolicy(Qt::ActionsContextMenu);

}

/*** System tray stuffz **/

/**
 * @brief Phonograph::createTrayIcon
 * Initialize tray icon in window constructor
 */
void Phonograph::createTrayIcon() {

    // Initiliaze and set icon
    this->trayIcon = new QSystemTrayIcon();
    this->trayIcon->setIcon( QIcon(":/images/background/theme/oud01.ico") );
    this->trayIcon->setToolTip( "Phonograph" );

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

}

/**
 * @brief Phonograph::iconActivated
 * @param reason
 * When the tray icon is clicked
 */
void Phonograph::iconActivated(QSystemTrayIcon::ActivationReason reason) {

    // Get action list
    QList< QAction * > actions = this->trayIcon->contextMenu()->actions();

    switch (reason) {
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::DoubleClick:
            this->restoreWindow();
            break;
        case QSystemTrayIcon::MiddleClick:
             //this->showMessage();
            break;

        case QSystemTrayIcon::Context:

            if (actions.count() > 0) {
                // Modify the play/pause action
                actions[1]->setText( (this->player->state() == QMediaPlayer::PlayingState) ? "Pause" : "Play" );
                actions[1]->setIcon( (this->player->state() == QMediaPlayer::PlayingState) ? QIcon(":/multimedia/player/icons/media-playback-pause-8.png") : QIcon(":/multimedia/player/icons/media-playback-start-8.png") );

                // Modify the stop
                actions[2]->setDisabled( !(this->player->state() == QMediaPlayer::PlayingState) );
            }
            break;
       default:
            break;
    }

}

/**
 * @brief restoreWindow
 * Restores the window when it's minimized to tray
 */
void Phonograph::restoreWindow() {

    // Show window / hide tray icon
    this->show();
    this->trayIcon->hide();
    this->wasMinimized = false;

}

void Phonograph::showMessage() {

    trayIcon->showMessage("Phonograph", "Rebetiko", QSystemTrayIcon::Information, 3000);

}

/**
 * @brief Phonograph::spawnTrayMenu
 * Build menu for tray icon
 */
void Phonograph::spawnTrayMenu() {

    // Create actions
    QAction *previous = new QAction("Previous", this->trayIcon);
    previous->setIcon( QIcon(":/multimedia/player/icons/media-skip-backward-8.png") );

    QAction *play_pause = new QAction( "Play" , this->trayIcon);
    play_pause->setIcon( QIcon(":/multimedia/player/icons/media-playback-start-8.png") );

    QAction *stop = new QAction("Stop", this->trayIcon);
    stop->setIcon( QIcon(":/multimedia/player/icons/media-playback-stop-8.png") );

    QAction *next = new QAction("Next", this->trayIcon);
    next->setIcon( QIcon(":/multimedia/player/icons/media-skip-backward-8.png") );

    QAction *restore = new QAction("Restore", trayIcon);

    QAction *quit = new QAction("Quit", this->trayIcon);
    quit->setIcon( QIcon(":/phonograph/menu/icons/menu/application-exit-2.png") );

    // Connect the signals
    connect(previous, SIGNAL(triggered()), this, SLOT(on_skip_backward_clicked()));
    connect(play_pause, SIGNAL(triggered(bool)), this, SLOT(on_play_clicked(bool)));
    connect(stop, SIGNAL(triggered()), this, SLOT(on_stop_clicked()));
    connect(next, SIGNAL(triggered()), this, SLOT(on_skip_forward_clicked()));
    connect(restore, SIGNAL(triggered()), this, SLOT(restoreWindow()));
    connect(quit, SIGNAL(triggered()), this, SLOT(on_actionQuit_triggered()));

    // Create the menu
    QMenu *trayMenu = new QMenu();
    trayMenu->addAction( previous );
    trayMenu->addAction( play_pause );
    trayMenu->addAction( stop );
    trayMenu->addAction( next );
    trayMenu->addSeparator();
    trayMenu->addAction( restore );
    trayMenu->addAction( quit );

    // Set the menu
    this->trayIcon->setContextMenu( trayMenu );

}

/**
 * @brief Phonograph::closeEvent
 * @param event
 * Override close event (user presses X on top right)
 */
void Phonograph::closeEvent (QCloseEvent *event) {

    // Ignore the default action
    event->ignore();

    // Show system tray icon
    this->trayIcon->show();

    // Hide window
    this->hide();

    // Set flag
    this->wasMinimized = true;

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

    // Set the label
    this->ui->endTimeLabel->setText( QString().sprintf("%02d", minutes) + QString(":") + QString().sprintf("%02d", seconds) );

    // Change the seeker range to achieve smoother scrolling. Set to tenths of seconds.
    this->ui->seeker->setRange(0, round(duration / 100) - 1);

}

/* Function used as slot for enabling media seeking with QSlider */
void Phonograph::setMediaPosition(int position) {

    if (this->player->isSeekable()){
        this->player->setPosition(100 * position);
    }

}

/* Slot that receives a signal when the media changes, so that several ui elements are updated */
void Phonograph::setPlayingSongLabel(QMediaContent content) {

    if (content.isNull() == false) {

        QPlaylistItem *currItem = (QPlaylistItem*)this->ui->playlist->item( this->playlist->currentIndex() );
        this->ui->playingNowLabel->setText(currItem->song.composer + " - " + currItem->song.title);
        this->ui->playingNowLabel->setAlignment(Qt::AlignCenter);
        QFont font("MS Shell Dlg 2", 12, QFont::Bold);
        this->ui->playingNowLabel->setFont(font);

        // Set current row in playlist
        this->ui->playlist->setCurrentRow( this->playlist->currentIndex() );

        // Fetch wiki article on composer or performer if composer is unknown or empty or nothing if both are empty
        if (currItem->song.composer.length() > 0 && currItem->song.composer.compare("Unknown") != 0) {

            this->fetchWikiArticle( currItem->song.composer );

        } else if (currItem->song.performer1.length() > 0) {

            this->fetchWikiArticle( currItem->song.performer1 );

        }

        // Set the Playing now tab stuffz
        this->ui->nowTitle->setText( currItem->song.title );
        this->ui->nowComposer->setText( currItem->song.composer );
        this->ui->nowPerformer->setText( currItem->song.performer1 );


    } else { //the playlist has finished

        this->ui->play->setChecked(false);

    }
}

/**
 * Update function
*/
bool Phonograph::updateLibrary() {
    // Show status message
    showStatus( "Syncing database..." );

    this->ui->library->clear();
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

        this->on_categorizeBySelect_currentIndexChanged( this->ui->categorizeBySelect->currentIndex() );

    } else {
        qDebug() << this->library->getLastError();
    }

    hideStatus();
    return true;
}

void Phonograph::addItemToLibrary(QTreeWidgetItem *topLevel, Song song, int categorizeBy) {

    // Create the new item
    QSongItem *newSong = new QSongItem();
    newSong->setText(0, song.title);
    newSong->setIcon(0, QIcon(":/phonograph/general/icons/songbird.png"));
    newSong->song = song;


    QString categorizeByTerm;
    if (categorizeBy == 0) {
        categorizeByTerm = song.composer;
    } else {
        categorizeByTerm = song.performer1;
    }

    // Look if the same artist is there
    int j, found = -1;
    for (j = 0; j < topLevel->childCount(); j++) {
        if ( topLevel->child(j)->text(0) == categorizeByTerm) {
            found = j;
            break;
        }
    }

    QTreeWidgetItem *newArtist;
    if (found == -1) {

        // If artist not found there, add it
        newArtist = new QTreeWidgetItem();
        newArtist->setText(0, categorizeByTerm);
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

/**
 * @brief Phonograph::addItemToPlaylist
 * @param song
 * Adds a new song to the playlist
 */
void Phonograph::addItemToPlaylist(Song song) {
    // Create the item object
    QPlaylistItem *newItem = new QPlaylistItem();
    newItem->setText( song.composer + QString(" - ") + song.performer1 + QString(" - ") + song.title );
    newItem->setIcon( QIcon(":/phonograph/general/icons/songbird.png") );
    newItem->song = song;


    // ...put the item on the GUI playlist...
    this->ui->playlist->addItem( newItem );
    // ...and to the media playlist
    QString url = this->library->getFilename( song.id );
    this->playlist->addMedia( QUrl(url) );
}

/**
 * @brief Phonograph::addItemsToPlaylist
 * @param songs
 * Adds many songs to the playlist
 */
void Phonograph::addItemsToPlaylist(QList<Song> songs) {

    // Get all the songs IDs
    QList<int> ids;
    for (int i = 0; i < songs.count(); i++) {
        ids.push_back( songs[i].id );
    }

    QHash<int, QString> filenames = this->library->getFilename( ids );

    if (filenames.size() == songs.size()) {

        for (int i = 0; i < songs.size(); i++) {

            // Create the item object
            QPlaylistItem *newItem = new QPlaylistItem();
            newItem->setText( songs[i].composer + QString(" - ") + songs[i].performer1 + QString(" - ") + songs[i].title );
            newItem->setIcon( QIcon(":/phonograph/general/icons/songbird.png") );
            newItem->song = songs[i];

            // ...put the item on the GUI playlist...
            this->ui->playlist->addItem( newItem );
            // ...and to the media playlist
            this->playlist->addMedia( QUrl( filenames.value( songs[i].id ) ) );

        }

    }
}

/**
 * @brief Phonograph::updatePlaylist
 * This function updates the QMediaPlaylist object with the list's contents
 * Shouldn't be called everytime the playlist has been changed but only when it is loaded from scratch
 */
void Phonograph::updatePlaylist() {
    this->playlist->clear();

    for (int i = 0; i < this->ui->playlist->count(); i++) {
        QPlaylistItem *item = dynamic_cast<QPlaylistItem *>(this->ui->playlist->item(i));
        if (item) {
            QString url = this->library->getFilename( item->song.id );
            this->playlist->addMedia( QUrl(url) );
        }
    }

    this->player->setPlaylist(this->playlist);
}

/**
 * @brief Phonograph::loadPlaylists
 * List all of the users playlists on disk
 */
void Phonograph::loadPlaylists() {

    QDir directory( QCoreApplication::applicationDirPath() + QString("/playlists"));
    QStringList files = directory.entryList( QStringList("*.spl") );
    this->ui->savedPlaylists->clear();

    for (int i = 0; i < files.count(); i++) {

        QListWidgetItem *newItem = new QListWidgetItem();
        newItem->setText( files[i].left( files[i].indexOf(".") ) );
        newItem->setIcon( QIcon(":/phonograph/general/icons/folder-multimedia.png") );        
        this->ui->savedPlaylists->addItem( newItem );

    }
}

/**
 * @brief Phonograph::loadPlaylist
 * @param name
 * Load the user playlist named "name" from disk
 */
void Phonograph::loadPlaylist(QString name) {

        // Create a playlist object
        this->selectedPlaylist.setName( name );

        // Load the playlist
        this->selectedPlaylist.load();
        QList<Song> contents = this->selectedPlaylist.getPlaylist();

        // Stop player in case it's playing the current playlist and clear the latter
        this->on_stop_clicked();
        this->ui->playlist->clear();

        // Add the songs
        this->addItemsToPlaylist( contents );

}

/**
 * @brief Phonograph::savePlaylist
 * @param name
 * Save playlist to disk
 */
void Phonograph::savePlaylist(QString name) {

    // Get song list from playlist items located on the list widget
    QList<Song> contents;

    for (int i = 0; i < this->ui->playlist->count(); i++) {
        QPlaylistItem *item = dynamic_cast<QPlaylistItem *>(this->ui->playlist->item(i));
        if (item) {
            contents.push_back( item->song );
        }
    }

    // Set the name and contents to the playlist object and save
    this->selectedPlaylist.setName( name );
    this->selectedPlaylist.setPlayist( contents );
    this->selectedPlaylist.save();
    this->loadPlaylists();
}

/**
 * @brief Phonograph::fetchWikiArticle
 * @param composer
 * Fetch wiki article for composer from rebetiko sealabs
 */
void Phonograph::fetchWikiArticle(QString composer) {
    //QString lang = this->ui->wikipedia_select_lang->currentText();
    //QString request_url = QString("http://") + lang + QString(".wikipedia.org/wiki/") + composer.replace( QString(" "), QString("_") );
    QString request_url = QString("http://rebetiko.sealabs.net/wiki/mediawiki/index.php/") + composer.replace( QString(" "), QString("_") );

    this->ui->wiki_view->setUrl( request_url );
}

/**
 * @brief Phonograph::fetchLyrics
 * @param title
 * Send a request to rebetiko sealabs to fetch the lyrics for the song if they are uploaded
 */
void Phonograph::fetchLyrics(QString title) {
    // Set the URL
    QString request_url = QString("http://rebetiko.sealabs.net/wiki/mediawiki/index.php/") + title.replace( QString(" "), QString("_") );

    // Initialize a network manager and connect the signal
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(parseLyrics(QNetworkReply*)));

    // Make GET request
    manager->get( QNetworkRequest( QUrl(request_url) ) );
}

/**
 * @brief Phonograph::parseLyrics
 * @param reply
 * Parse the reply to the lyrics request
 */
void Phonograph::parseLyrics(QNetworkReply* reply) {

    // Get the contents
    QString htmlContents( reply->readAll() );

    // Parse to get the bodyContent element
    QWebPage page;
    page.mainFrame()->setHtml( htmlContents );
    QWebElement parse = page.mainFrame()->documentElement();
    QWebElement result = parse.findFirst("div[id=bodyContent]");

    if (result.isNull()) {
        return;
    }

    // Strip HTML
    QString lyrics = result.toInnerXml();
    qDebug() << lyrics.indexOf("<!-- Saved");
    lyrics = lyrics.mid( lyrics.indexOf("</h2>") + 5 , lyrics.indexOf("<div") - lyrics.indexOf("</h2>") - 5 );
    lyrics = lyrics.replace( QRegExp("<<?.>") , "" ).replace( QRegExp("</<?.>") , "" ).replace( QRegExp("<!--.*-->") , "" ).replace( QRegExp("<.*>") , "" );

    // Print the lyrics
    this->ui->lyrics_display->setText( lyrics );
}

void Phonograph::showStatus(QString msg) {

    if (this->isDialogShown == true) {
        hideStatus();
    }

    // Create the dialog and set its settings
    this->statusDialog = new QDialog( this );
    this->statusDialog->setWindowOpacity( 0.70 );
    this->statusDialog->setWindowFlags( Qt::SplashScreen );
    this->statusDialog->setWindowModality( Qt::WindowModal );

    // Fix a layout for it
    this->statusDialog->setLayout( new QVBoxLayout() );

    // Add the label with the status message
    QLabel *label = new QLabel(  );
    label->setFont( QFont("MS Shell Dlg 2", 15, QFont::Bold, true) );
    label->setText( msg );
    this->statusDialog->layout()->addWidget( label );

    // Show the dialog
    this->statusDialog->show();

    this->isDialogShown = true;

}

void Phonograph::hideStatus() {

    this->statusDialog->close();
    this->isDialogShown = false;

}

/**************/
/**** Events ****/
/**************/

void Phonograph::deletePlaylist() {

    // Get the selected playlist
    QString playlist = this->ui->savedPlaylists->currentItem()->text();

    // Delete it
    QPlaylist::deletePlaylist( playlist );

    // Delete the item from the list
    delete this->ui->savedPlaylists->item( this->ui->savedPlaylists->currentRow() );

}

void Phonograph::on_library_itemDoubleClicked(QTreeWidgetItem *item, int column) {

    QSongItem *itemClicked = dynamic_cast<QSongItem *>(item);

    if (itemClicked) {

        this->addItemToPlaylist( itemClicked->song );

    } else {

        QList< Song > allSongs;
        for (int i = 0; i < item->childCount(); i++) {

            itemClicked = dynamic_cast<QSongItem *>(item->child(i));
            if (itemClicked) {
                allSongs.push_back( itemClicked->song );
            }

        }

        if (allSongs.size() > 0) {
            this->addItemsToPlaylist( allSongs );
        }

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

        }  else if (!this->playlist->isEmpty()) {

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
/*
    this->playlist->addMedia( QUrl("http://echidna-band.com/manifest/mp3/Manifests_Of_Human_Existence/08-Pendulum.mp3") );
    this->playlist->addMedia( QUrl::fromLocalFile("/home/verminoz/Music/giaf-giouf.mp3") );

    this->playlist->setCurrentIndex(0);

    this->player->play();
    qDebug() << "State: " << player->state();
    qDebug() << "Media State: " << player->mediaStatus();
    qDebug() << "Error: " << player->error() << " " << player->errorString();
    qDebug() << "Error: " << playlist->error() << " " << playlist->errorString();
*/
   if(this->playlist->previousIndex() != -1) {

        this->playlist->previous();

    }

}

void Phonograph::on_skip_forward_clicked() {

    if(this->playlist->nextIndex() != -1) {

        this->playlist->next();

    }

}

void Phonograph::on_clearPlaylist_clicked() {
    this->ui->playlist->clear();
    updatePlaylist();
}

void Phonograph::on_addPlaylistItem_clicked() {
    // Get the selected items
    QList<QTreeWidgetItem *> selectedSongs = this->ui->library->selectedItems();

    // Loop through selected items
    for (int i = 0; i < selectedSongs.count(); i++){
        // Attempt to cast to QSongItem
        QSongItem *itemSelected = dynamic_cast<QSongItem *>(selectedSongs[i]);


        if (itemSelected) {

            // If casting succeeded, it means that it was actually a song
            this->addItemToPlaylist( itemSelected->song );

        } else {

            // If not then it means it was a full category (composer/performer) and thus we will loop through and call the double click function to add them
            for (int j = 0; j < selectedSongs[i]->childCount(); j++) {
                this->on_library_itemDoubleClicked(selectedSongs[i]->child(j), 0);
            }

        }
    }
}

void Phonograph::on_removePlaylistItem_clicked() {
    QList<QListWidgetItem *> selectedSongs = this->ui->playlist->selectedItems();

    if (!this->playlist->isEmpty()) {

        for(int i = 0; i < selectedSongs.count(); i++){
            QPlaylistItem *itemSelected = dynamic_cast<QPlaylistItem *>(selectedSongs[i]);

            for(int j = 0; j < this->ui->playlist->count(); j++) {

                QPlaylistItem *playlistItem = dynamic_cast<QPlaylistItem *>(this->ui->playlist->item(j));
                if (itemSelected->song.id == playlistItem->song.id) {
                    delete this->ui->playlist->item(j);
                    this->playlist->removeMedia(j);
                }
            }

        }

    }

}

void Phonograph::on_shuffle_clicked(bool checked) {

    if (checked) {

        this->ui->toolButton->setChecked(false);
        this->playlist->shuffle();

    } else {

        this->playlist->setPlaybackMode(QMediaPlaylist::Sequential);

    }

}

void Phonograph::on_toolButton_clicked(bool checked) {

    if (checked) {

        this->ui->shuffle->setChecked(false);
        this->playlist->setPlaybackMode(QMediaPlaylist::Loop);

    }
    else {

        this->playlist->setPlaybackMode(QMediaPlaylist::Sequential);

    }

}

void Phonograph::on_seek_forward_clicked() {

    int pos = this->ui->seeker->sliderPosition();
    setMediaPosition(pos + 100);

}

void Phonograph::on_seek_backward_clicked() {

    int pos = this->ui->seeker->sliderPosition();
    setMediaPosition(pos - 100);

}

// About dialog trigger
void Phonograph::on_actionAbout_Phonograph_triggered() {

    AboutDialog *about = new AboutDialog(this);
    about->setFixedSize(778, 437);
    about->show();

}

void Phonograph::on_savePlaylist_clicked() {

    QString name = this->ui->playlistName->text();

    if (name.length() == 0) {
        QMessageBox msgbox;
        msgbox.setText( "Please enter a name for your playlist" );
        msgbox.exec();
        this->ui->playlistName->setFocus();
        return;
    }

    this->savePlaylist( name );

}

void Phonograph::on_searchLibraryText_textChanged(const QString &arg1) {

    // Check on which tab we are to search the appropriate list
    if (this->ui->libraryTabWidget->currentIndex() == 0) {

        // Take top level item of library
        QTreeWidgetItem* topLevel = this->ui->library->topLevelItem(0);

        // Loop through all of its items
        for (int i = 0; i < topLevel->childCount(); i++) {

            // If we matched the composer then show the composer and all of the songs and continue to next composer
            if ( topLevel->child(i)->text(0).contains(arg1) ) {
                topLevel->child(i)->setHidden( false );
                for (int j = 0; j < topLevel->child(i)->childCount(); j++) {
                    topLevel->child(i)->child(j)->setHidden( false );
                }
                continue;
            }

            // If we didn't match the composer's name let's check if we can match any of the songs
            bool isThereMatch = false;
            for (int j = 0; j < topLevel->child(i)->childCount(); j++) {

                 topLevel->child(i)->child(j)->setHidden( ! topLevel->child(i)->child(j)->text(0).contains( arg1 ) );
                 isThereMatch |= topLevel->child(i)->child(j)->text(0).contains( arg1 );

            }

            // If there was a song matched then the composer should be visible, if not then not :P
            topLevel->child(i)->setHidden( !isThereMatch );
        }

    } else {

        for (int i = 0; i < this->ui->savedPlaylists->count(); i++) {

            QListWidgetItem *item = this->ui->savedPlaylists->item(i);
            item->setHidden( !item->text().contains( arg1 ) );

        }

    }

}

void Phonograph::on_searchPlaylistText_textChanged(const QString &arg1) {

    for (int i = 0; i < this->ui->playlist->count(); i++) {

        QListWidgetItem *item = this->ui->playlist->item(i);
        item->setHidden( !item->text().contains( arg1 ) );

    }

}

void Phonograph::on_searchLibraryClear_clicked() {

    this->ui->searchLibraryText->clear();

}

void Phonograph::on_searchPlaylistClear_clicked() {

    this->ui->searchPlaylistText->clear();

}

void Phonograph::on_savedPlaylists_itemDoubleClicked(QListWidgetItem *item) {

    this->ui->playlistName->setText( item->text() );
    this->loadPlaylist( item->text() );

}

void Phonograph::on_categorizeBySelect_currentIndexChanged(int index) {
    // Disable left sidebar since it shouldn't be touchable while updating
    // but also to give the user the knowledge that it is updating
    this->ui->sidebarleft->setEnabled( false );
    showStatus( "Sorting..." );

    // Clear library first
    this->ui->library->clear();

    // Add top level item
    QTreeWidgetItem *topLevel = new QTreeWidgetItem( this->ui->library );
    if (index == 0) {
        topLevel->setText(0, "Composers"); // 0 --> Categorize by composers
    } else {
        topLevel->setText(0, "Singers"); // 1 --> Categorize by singers
    }
    topLevel->setIcon(0, QIcon(":/phonograph/general/icons/folder-sound.png"));

    // Loop through the songs
    for (int i = 0; i < this->library->songs.length(); i++) {

        this->addItemToLibrary( topLevel , this->library->songs[i] , index );

    }

    // Sort and expand
    this->ui->library->sortItems(0, Qt::AscendingOrder);
    this->ui->library->expandItem( topLevel );

    // Re-enable the left sidebar
    this->ui->sidebarleft->setEnabled( true );
    hideStatus();
}

void Phonograph::on_actionQuit_triggered() {

    // Quit the application
    QApplication::quit();

}
