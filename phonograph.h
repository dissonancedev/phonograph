#ifndef PHONOGRAPH_H
#define PHONOGRAPH_H

#include <cmath>
#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QFile>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QMediaPlaylist>
#include <QSettings>
#include <QCoreApplication>
#include <QDir>
#include <QMessageBox>
#include <QTimer>
#include "system/musicdatabase.h"
#include "system/qsongitem.h"
#include "system/qplaylistitem.h"
#include "system/qplaylist.h"
#include "aboutdialog.h"

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
    void addItemsToPlaylist(QList<Song> songs);
    void addItemToLibrary(QTreeWidgetItem *topLevel, Song song, int categorizeBy);
    void updatePlaylist();

    // Playlist functiom
    void loadPlaylists();
    void loadPlaylist(QString name);
    void savePlaylist(QString name);

    // Fetch wikipedia article
    void fetchWikiArticle(QString composer);

    // Status dialog
    void showStatus(QString msg);
    void hideStatus();

private slots:

    void setPlaybackTimer(qint64 position);

    void setMediaTime(qint64 duration);

    void setMediaPosition(int position);

    void on_library_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_stop_clicked();

    void on_volume_valueChanged(int value);

    void on_mute_toggled(bool checked);

    void on_play_clicked(bool checked);

    void on_playlist_itemDoubleClicked(QListWidgetItem *item);

    void on_skip_backward_clicked();

    void on_skip_forward_clicked();

    void on_clearPlaylist_clicked();

    void on_addPlaylistItem_clicked();

    void on_removePlaylistItem_clicked();

    void setPlayingSongLabel(QMediaContent content);

    void on_shuffle_clicked(bool checked);

    void on_toolButton_clicked(bool checked);

    void on_seek_forward_clicked();

    void on_seek_backward_clicked();

    void on_actionAbout_Phonograph_triggered();

    void on_wikipedia_select_lang_currentTextChanged(const QString &arg1);

    void on_savePlaylist_clicked();

    void on_searchLibraryText_textChanged(const QString &arg1);

    void on_searchPlaylistText_textChanged(const QString &arg1);

    void on_searchLibraryClear_clicked();

    void on_searchPlaylistClear_clicked();

    void on_savedPlaylists_itemDoubleClicked(QListWidgetItem *item);

    void on_categorizeBySelect_currentIndexChanged(int index);

    void on_savedPlaylists_itemClicked(QListWidgetItem *item);

    void afterShowEvent();

private:
    // UI variable
    Ui::Phonograph *ui;

    // Music library variable
    MusicDatabase *library;

    // Media player stuff
    QMediaPlayer *player;
    QMediaPlaylist *playlist;

    // Playlist handler
    QPlaylist selectedPlaylist;

    // Settings implementation function
    void loadSettings();
    void saveSettings();

    // Message dialog
    QDialog *statusDialog = NULL;
    bool isDialogShown = false;

protected:
    void showEvent(QShowEvent *event);
};

#endif // PHONOGRAPH_H
