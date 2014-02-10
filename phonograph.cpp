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

    this->library = new MusicDatabase(host, port, username, password, dbname);
    if (this->library->update()) {
        qDebug() << "Found " << this->library->songs.length() << " songs";
        // Add top level item
        QTreeWidgetItem *topLevel = new QTreeWidgetItem( this->ui->library );
        topLevel->setText(0, "Artists");
        topLevel->setIcon(0, QIcon(":/phonograph/general/icons/folder-sound.png"));

        int i,j;

        for (i = 0; i < this->library->songs.length(); i++) {

            // Look if the same artist is there
             int found = -1;
             for (j = 0; j < topLevel->childCount(); j++) {
                 if ( topLevel->child(j)->text(0) == this->library->songs[i].composer) {
                     found = j;
                     break;
                 }
             }

             QTreeWidgetItem *newSong = new QTreeWidgetItem();
             newSong->setText(0, this->library->songs[i].title);
             newSong->setIcon(0, QIcon(":/phonograph/general/icons/songbird.png"));


             if (found == -1) {

                 // If artist not found there add it
                 QTreeWidgetItem *newArtist = new QTreeWidgetItem();
                 newArtist->setText(0, this->library->songs[i].composer);
                 topLevel->addChild( newArtist );

                 // Add icon to it
                 newArtist->setIcon(0, QIcon(":/phonograph/general/icons/view-media-artist.png"));

                 // Finally add the song to it
                 newArtist->addChild( newSong );

             } else {

                topLevel->child(j)->addChild( newSong );

             }

        }
    } else {
        qDebug() << this->library->getLastError();
    }

    return true;
}
