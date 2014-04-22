#include "phonograph.h"
#include "system/musicdatabase.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Set application icon
    a.setWindowIcon( QIcon(":/images/background/theme/oud01.ico") );

    // Initialize main form
    Phonograph w;

    // Set icon and show main window
    w.setWindowIcon( QIcon(":/images/background/theme/oud01.ico") );
    w.show();

    return a.exec();
}
