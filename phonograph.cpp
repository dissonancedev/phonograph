#include "phonograph.h"
#include "ui_phonograph.h"

/*** Constructor & destructor ***/

Phonograph::Phonograph(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Phonograph)
{
    // Setup the UI
    ui->setupUi(this);

    ui->mainframe->setStyleSheet( "QFrame#mainframe { background-image: url(:/images/background/theme/diskos-25-leivadia.jpg); }" );

    // Update library
    this->updateLibrary();
}

Phonograph::~Phonograph()
{
    delete ui;
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
    QListWidgetItem *newItem = new QListWidgetItem();
    newItem->setText( song.composer + QString(" - ") + song.year + QString(" - ") + song.title );
    newItem->setIcon( QIcon(":/phonograph/general/icons/songbird.png") );
    this->ui->playlist->addItem( newItem );
}


void Phonograph::on_library_itemDoubleClicked(QTreeWidgetItem *item, int column) {
    QSongItem *itemClicked = dynamic_cast<QSongItem *>(item);

    if (itemClicked) {
        this->addItemToPlaylist( itemClicked->song );
    }
}
