#include "phonograph.h"
#include "system/musicdatabase.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon( QIcon(":/images/background/theme/oud01.ico") );
    Phonograph w;
    w.setWindowIcon( QIcon(":/images/background/theme/oud01.ico") );
    w.show();

    return a.exec();
}
