#ifndef PHONOGRAPH_H
#define PHONOGRAPH_H

// Debug stuff
#include <QDebug>
#include <QThread>

// Build stuff
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
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QWebPage>
#include <QWebFrame>
#include <QWebElement>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QTranslator>
#include <QProgressDialog>
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

    // Fetch lyrics
    void fetchLyrics(QString title);

    // Status dialog
    void showStatus(QString msg);
    void hideStatus();

    // Fix popup menu for playlist list
    void createPlaylistPopup();

    // System tray icon
    void createTrayIcon();
    void spawnTrayMenu();

    // Language
    void switchLanguage();

private slots:

    // System tray
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void showMessage();
    void restoreWindow();

    // Status dialog
    void executeStatus();

    // Parse lyrics
    void parseLyrics(QNetworkReply* reply);

    // Delete playlist
    void deletePlaylist();

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

    void on_savePlaylist_clicked();

    void on_searchLibraryText_textChanged(const QString &arg1);

    void on_searchPlaylistText_textChanged(const QString &arg1);

    void on_searchLibraryClear_clicked();

    void on_searchPlaylistClear_clicked();

    void on_savedPlaylists_itemDoubleClicked(QListWidgetItem *item);

    void on_categorizeBySelect_currentIndexChanged(int index);

    void afterShowEvent();

    void on_actionQuit_triggered();

    void on_actionEnglish_triggered(bool checked);

    void on_actionGreek_triggered(bool checked);

    void on_actionDansk_triggered(bool checked);

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
    QProgressDialog *progress;
    QDialog *statusDialog = NULL;
    bool isDialogShown;
    bool wasMinimized;

    // Language setting
    QString currentLanguage;

    // System tray icon
    QSystemTrayIcon *trayIcon;
    void closeEvent (QCloseEvent *event);

protected:
    void showEvent(QShowEvent *event);
};

#endif // PHONOGRAPH_H
