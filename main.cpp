#include "phonograph.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Phonograph w;
    w.show();

    return a.exec();
}
