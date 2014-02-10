#ifndef PHONOGRAPH_H
#define PHONOGRAPH_H

#include <QMainWindow>
#include <QFile>
#include "system/musicdatabase.h"

namespace Ui {
class Phonograph;
}

class Phonograph : public QMainWindow
{
    Q_OBJECT

public:
    explicit Phonograph(QWidget *parent = 0);
    ~Phonograph();

    bool updateLibrary();

private:
    // UI variable
    Ui::Phonograph *ui;

    // Music library variable
    MusicDatabase *library;
};

#endif // PHONOGRAPH_H
