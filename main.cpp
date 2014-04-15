#include "phonograph.h"
#include "system/musicdatabase.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon( QIcon(":/images/background/theme/bouzouki.ico") );
    Phonograph w;
    w.setWindowIcon( QIcon(":/images/background/theme/bouzouki.ico") );
    w.show();

    return a.exec();
}
