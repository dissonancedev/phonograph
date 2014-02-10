#include "phonograph.h"
#include "ui_phonograph.h"

/*** Constructor & destructor ***/

Phonograph::Phonograph(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Phonograph)
{
    // Setup the UI
    ui->setupUi(this);

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
    QFile credentialsFile("/home/verminoz/phonograph/src/bin/credentials.txt");
    credentialsFile.open(QFile::ReadOnly); // Open file
    QTextStream fileStream(&credentialsFile); // Set stream
    QString line = fileStream.readAll().trimmed(); // Get the data trimmed
    QString username = line.split(",")[0];
    QString password = line.split(",")[1];

    this->library = new MusicDatabase(host, port, username, password, dbname);
    if (this->library->update()) {
        qDebug() << "Found " << this->library->songs.length() << " songs";
    } else {
        qDebug() << this->library->getLastError();
    }

    return true;
}
