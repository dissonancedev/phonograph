#ifndef PLAYLISTNAMEDIALOG_H
#define PLAYLISTNAMEDIALOG_H

#include <QDialog>
#include "phonograph.h"

namespace Ui {
class PlaylistNameDialog;
}

class PlaylistNameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlaylistNameDialog(QWidget *parent = 0);
    ~PlaylistNameDialog();

private slots:
    void on_buttonBox_accepted();

signals:
   void loadPlaylists();

private:
    Ui::PlaylistNameDialog *ui;
};

#endif // PLAYLISTNAMEDIALOG_H
