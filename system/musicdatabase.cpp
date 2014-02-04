#include "musicdatabase.h"

MusicDatabase::MusicDatabase() {
    this->database = 0;
}

MusicDatabase::MusicDatabase(QString url, int port, QString username, QString password, QString dbname) {
    MusicDatabase();

    // Set all the info
    this->setUrl(url);
    this->setPort(port);
    this->setUsername(username);
    this->setPassword(password);
    this->setDBName(dbname);

    // Try to connect
    this->connect();
}


MusicDatabase::~MusicDatabase() {
    if (this->isConnected()) {
        this->disconnect();
    }
}

/* Getters and setters */
// Getters
QString MusicDatabase::getUrl() {
    return this->url;
}

int MusicDatabase::getPort() {
    return this->port;
}

QString MusicDatabase::getUsername() {
    return this->username;
}

QString MusicDatabase::getDBName() {
    return this->dbname;
}

// Setters
void MusicDatabase::setUrl(QString url) {
    if (QUrl(url).isValid()) {
        this->url = url;
    }
}

void MusicDatabase::setPort(int port) {
    if (port > 0) {
        this->port = port;
    }
}

void MusicDatabase::setUsername(QString username) {
    this->username = username;
}

void MusicDatabase::setPassword(QString password) {
    this->password = password;
}

void MusicDatabase::setDBName(QString dbname) {
    this->dbname = dbname;
}

/* Error stack */
QList<QString> MusicDatabase::getErrors() {
    return this->errors;
}

QString MusicDatabase::getLastError() {
    if (!this->errors.isEmpty()) {
        return this->errors.last();
    }

    return QString("");
}

/* API functions */
bool MusicDatabase::connect() {
    // Create database connection object
    this->database = new QSqlDatabase();

    // Set all the settings
    this->database->setHostName ( this->url );
    this->database->setPort ( this->port );
    this->database->setUserName ( this->username );
    this->database->setPassword ( this->password );
    this->database->setDatabaseName ( this->dbname );

    // Open the connection
    if (!this->database->open()) {
        this->errors.push_back( QString(this->database->lastError().number()) + QString(": ") + QString((this->database->lastError().text())) );
    }

    return this->database->isOpen();
}

bool MusicDatabase::update() {
    return true;
}

void MusicDatabase::disconnect() {
    // Check if object exists
    if (this->database) {
        this->database->close();
        delete this->database;
        this->database = 0;
    }
}

bool MusicDatabase::isConnected() {
    if (this->database) {
        return this->database->isOpen();
    }

    return false;
}
