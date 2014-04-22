#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::switchLanguage() {

    // Get the app directory
    QString appDir = QApplication::applicationDirPath();

    // Setup translator
    QTranslator phonographTranslator;
    if (phonographTranslator.load(appDir + QString("/languages/phonograph_%1.qm").arg(this->currentLanguage))) {
        qApp->installTranslator(&phonographTranslator);
         this->ui->retranslateUi(this);
    }

}
