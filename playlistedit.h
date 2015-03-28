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

private slots:
    void on_removeSong_clicked();

    void on_buttonBox_accepted();

private:
    Ui::PlaylistEditDialog *ui;
    QListWidgetItem *item;
    QPlaylist selectedPlaylist;
};

#endif // PLAYLISTEDIT_H
