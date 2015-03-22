#include "authentication.h"

static bool login(QString username, QString password) {

    // Create database connection object
    QSqlDatabase database = QSqlDatabase::addDatabase("QMYSQL");

    /*database.setHostName (  );
    database.setPort (  );
    database.setUserName (  );
    database.setPassword ( );
    database.setDatabaseName (  );
*/
    // Open the connection
    if (!database.open()) {

        qDebug() << QString(database.lastError().number()) + QString(": ") + QString(database.lastError().text());

        return false;
    }

    qDebug() << "Connected to database";

    QString sqlQuery = QString("SELECT password FROM phpbb2.public.song WHERE username = :uname");

    // Execute the query
    QSqlQuery resultSet;
    resultSet.prepare( sqlQuery );
    resultSet.bindValue(":uname", username);
    resultSet.exec();

    // If login is successfull
    bool success = false;
    QString storedPassword;

    // Check if the resultset has any items
    if (resultSet.size() > 0) {

        // Add all items to the list again
        while (resultSet.next()) {

            // Set all the data on it
            storedPassword = resultSet.value(0).toString();

        }

        success = (password == storedPassword);

    } else {

        qDebug() << "Couldn't find user";

    }

    database.close();
    QSqlDatabase::removeDatabase("QMYSQL");

    qDebug() << "Disconnected from database";

    return success;

}
