#ifndef PHONOGRAPH_H
#define PHONOGRAPH_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QFile>
#include "system/musicdatabase.h"
#include "system/qsongitem.h"

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
    void addItemToPlaylist(Song song);

private slots:

    void on_library_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    // UI variable
    Ui::Phonograph *ui;

    // Music library variable
    MusicDatabase *library;
};

#endif // PHONOGRAPH_H
