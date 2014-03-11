#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <QDebug>

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class Authentication
{
public:
    static bool login(QString username, QString password);
};

#endif // AUTHENTICATION_H
