#include "musicdatabase.h"

MusicDatabase::MusicDatabase() {
    this->database = 0;
}

MusicDatabase::MusicDatabase(QString url, int port, QString username, QString password, QString dbname) {
    MusicDatabase();

    // Set all the info
    this->setHost(url);
    this->setPort(port);
    this->setUsername(username);
    this->setPassword(password);
    this->setDBName(dbname);
}


MusicDatabase::~MusicDatabase() {
    if (this->isConnected()) {
        this->disconnect();
    }
}

/* Getters and setters */
// Getters
QString MusicDatabase::getHost() {
    return this->host;
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
void MusicDatabase::setHost(QString host) {
    if (QUrl(host).isValid()) {
        this->host = host;
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
    this->database->setHostName ( this->host );
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
    // Try to connect
    if (this->connect()) {
        // Query to fetch all songs
        QString sqlQuery = "SELECT rec_id, file_name, name, description, date_rec, composer, singer1, singer2 FROM phpbb2.public.song";

        // Execute the query
        QSqlQuery resultSet = this->database->exec(sqlQuery);

        // Check if the resultset has any items
        if (resultSet.size() > 0) {

            // Clear list
            this->songs.clear();

            // Add all items to the list again
            while (resultSet.next()) {
                // Create a temporary song object
                Song temp;

                // Set all the data on it
                temp.id = resultSet.value(0).toInt();
                temp.filename = resultSet.value(1).toString();
                temp.title = resultSet.value(2).toString();
                temp.info = resultSet.value(3).toString();
                temp.year = resultSet.value(4).toString();
                temp.composer = resultSet.value(5).toString();
                temp.performer1 = resultSet.value(6).toString();
                temp.performer2 = resultSet.value(7).toString();

                // Append it to the list
                this->songs.push_back(temp);
            }

        }

        this->disconnect();

        return true;
    }

    return false;
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
