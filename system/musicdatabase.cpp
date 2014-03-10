#include "musicdatabase.h"

MusicDatabase::MusicDatabase() {

}

MusicDatabase::MusicDatabase(QString url, int port, QString username, QString password, QString dbname) {
    MusicDatabase();

    // Set all the info
    this->setHost(url);
    this->setPort(port);
    this->setUsername(username);
    this->setPassword(password);
    this->setDBName(dbname);

    // Create database connection object
    this->database = QSqlDatabase::addDatabase("QPSQL");
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

    // Set all the settings
    this->database.setHostName ( this->host );
    this->database.setPort ( this->port );
    this->database.setUserName ( this->username );
    this->database.setPassword ( this->password );
    this->database.setDatabaseName ( this->dbname );

    // Open the connection
    if (!this->database.open()) {
        this->errors.push_back( QString(this->database.lastError().number()) + QString(": ") + QString((this->database.lastError().text())) );
        qDebug() << errors.last();
        return false;
    }

    qDebug() << "Connected to database";

    return this->database.isOpen();
}

bool MusicDatabase::update() {
    // Check first if we can load from cache
    this->cache = new DatabaseCache(); // Load cache

    // See if the number of records is the same
    if (this->cache->count() >= this->getRecordCount()) {
        // If they are equal we can just load from cache and return
        this->songs = this->cache->getContent();
        this->disconnect();
        qDebug() << "Loaded from cache";

        return true;
    }

    // Try to connect
    if (this->connect()) {

        // Query to fetch all songs
        QString sqlQuery = "SELECT rec_id, name, composer, singer1 FROM phpbb2.public.song";

        // Execute the query
        QSqlQuery resultSet = this->database.exec(sqlQuery);

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
                temp.title = resultSet.value(1).toString();

                // Fix it to have unknown in case it is an empty string
                if (resultSet.value(2).toString().isEmpty()) {
                    temp.composer = "Unknown";
                } else {
                    temp.composer = resultSet.value(2).toString();
                }

                // Fix empty performer labels
                if (resultSet.value(3).toString().isEmpty()) {
                    temp.performer1 = "Unknown";
                } else {
                    temp.performer1 = resultSet.value(3).toString();
                }

                // Append it to the list
                this->songs.push_back(temp);
            }

            // If we had to fetch let's also store it in cache
            this->cache->setContent( this->songs );
            this->cache->saveCache();

        } else {
            qDebug() << "Empty result set fetching songs";
        }

        this->disconnect();

        return true;
    }

    return false;
}

void MusicDatabase::disconnect() {
    // Check if object exists
    this->database.close();

    qDebug() << "Disconnected from database";

}

bool MusicDatabase::isConnected() {
    return this->database.isOpen();
}

int MusicDatabase::getRecordCount() {
    // Try to connect
    if (this->connect()) {
        QString sqlQuery = "SELECT count(*) FROM phpbb2.public.song";

        // Execute the query
        QSqlQuery resultSet = this->database.exec(sqlQuery);

        if (resultSet.size() > 0) {
            resultSet.first();
            int value = resultSet.value(0).toInt();
            this->disconnect();
            return value;
        }

    }

    return 0;
}

QString MusicDatabase::getFilename(int id) {

    if (this->connect()) {
        QString sqlQuery = QString("SELECT file_name FROM phpbb2.public.song WHERE rec_id = :id");

        // Execute the query
        QSqlQuery resultSet;
        resultSet.prepare( sqlQuery );
        resultSet.bindValue(":id", id);
        resultSet.exec();

        if (resultSet.size() > 0) {

            QString value;
            if (resultSet.next()) {
                value = Song::filename + normalizeUrl( resultSet.value(0).toString() );
            }

            // Disconnect from database
            this->disconnect();

            // Return value
            return value;

        }

    }

    return QString("");
}

QStringList MusicDatabase::getFilename(QList<int> ids) {

    if (this->connect()) {

        // Build a string with all the parameters for binding
        QStringList inValuesBindings;
        for (int i = 0; i < ids.count(); i++) {

            inValuesBindings.push_back( QString(":id") + QString(i) );

        }

        QString sqlQuery = QString("SELECT file_name FROM phpbb2.public.song WHERE rec_id IN (") + inValuesBindings.join(", ") + QString(")");

        // Prepare the query
        QSqlQuery resultSet;
        resultSet.prepare( sqlQuery );

        // Bind all values
        for (int i = 0; ids.count(); i++) {
            resultSet.bindValue(QString(":id") + QString(i), ids[i]);
        }

        // Execute the query
        resultSet.exec();

        if (resultSet.size() > 0) {

            QStringList result;
            while (resultSet.next()) {
                QString value = resultSet.value(0).toString();
                result.push_back( Song::filename + normalizeUrl( value ) );
            }

            // Disconnect from database
            this->disconnect();

            // Return the result
            return result;

        }

    }

    return QStringList("");

}

/* Function that encodes the greek filenames to the proper format */
QString MusicDatabase::normalizeUrl(QString url) {
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
    url.replace("΄", "%B4");

    return url;
}
