#ifndef PHONOGRAPH_H
#define PHONOGRAPH_H

#include <QMainWindow>

namespace Ui {
class Phonograph;
}

class Phonograph : public QMainWindow
{
    Q_OBJECT

public:
    explicit Phonograph(QWidget *parent = 0);
    ~Phonograph();

private:
    Ui::Phonograph *ui;
};

#endif // PHONOGRAPH_H
