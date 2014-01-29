#include "phonograph.h"
#include "ui_phonograph.h"

Phonograph::Phonograph(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Phonograph)
{
    ui->setupUi(this);
}

Phonograph::~Phonograph()
{
    delete ui;
}
