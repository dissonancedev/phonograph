#ifndef PLAYLISTSDIALOG
#define PLAYLISTSDIALOG

#include <QDialog>
#include <QApplication>
#include <QDir>
#include <QListWidgetItem>
#include <QListWidget>
#include "system/qplaylist.h"
#include "system/qlibrarywidget.h"
#include "system/qplaylistitem.h"
#include "playlistedit.h"

namespace Ui {
class PlaylistsDialog;
}

class PlaylistsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlaylistsDialog(QWidget *parent = 0, QString source = "library", QLibraryWidget *library = 0, QListWidget *playlist = 0);
    ~PlaylistsDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::PlaylistsDialog *ui;
    QPlaylist *playlistSelected;
    QList<QListWidgetItem *> selectedPlaylist;
    QString source;
    QLibraryWidget *library;
    QListWidget *playlist;
    QWidget* getParentWindow();

};

#endif // PLAYLISTSDIALOG

