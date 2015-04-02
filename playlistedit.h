#ifndef PLAYLISTEDIT_H
#define PLAYLISTEDIT_H

#include <QDialog>
#include <QListWidgetItem>
#include "phonograph.h"

namespace Ui {
class PlaylistEditDialog;
}

class PlaylistEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlaylistEditDialog(QWidget *parent = 0, QListWidgetItem *item = 0);
    ~PlaylistEditDialog();
    QListWidgetItem* selectedSong;

private slots:
    void on_removeSong_clicked();

    void on_buttonBox_accepted();

    void on_addToCurrent_clicked();

    void on_addToList_clicked();

private:
    Ui::PlaylistEditDialog *ui;
    QListWidgetItem *item;
    QPlaylist selectedPlaylist;
    QWidget* getParentWindow();

};

#endif // PLAYLISTEDIT_H
