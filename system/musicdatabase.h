#ifndef MUSICDATABASE_H
#define MUSICDATABASE_H

#include <QList>
#include "song.h"
#include <QtSql/QSqlDatabase>

class MusicDatabase {
private:
    QSqlDatabase *database;

    // Database login info
    QString url;
    int port;
    QString username;
    QString password;
    QString dbname;

    // Error stack
    QList<QString> errors;

public:
    /* Constructors & Destructors */
    MusicDatabase();
    MusicDatabase(QString url, int port, QString username, QString password, QString dbname);

    /* Getters and setters */
    // Getters
    QString getUrl();
    int getPort();
    QString getUsername();
    QString getDBName();
    // Setters
    void setUrl(QString url);
    void setUsername(QString username);
    void setPassword(QString password);
    void setDBName(QString password);

    /* Error stack */
    QList<QString> getErrors();
    QString getLastError();

    /* API functions */
    bool connect();
    bool update();
    bool disconnect();

    // QList<Song> songs;
};

#endif // MUSICDATABASE_H
