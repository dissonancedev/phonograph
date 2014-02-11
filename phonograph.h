#ifndef PHONOGRAPH_H
#define PHONOGRAPH_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QFile>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QMediaPlaylist>
#include "system/musicdatabase.h"
#include "system/qsongitem.h"
#include "system/qplaylistitem.h"

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
    void addItemToLibrary(QTreeWidgetItem *topLevel, Song song);
    void updatePlaylist();

private slots:

    void on_library_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_play_toggled(bool checked);

private:
    // UI variable
    Ui::Phonograph *ui;

    // Music library variable
    MusicDatabase *library;

    // Media player stuff
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
};

#endif // PHONOGRAPH_H
