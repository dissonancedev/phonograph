#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include <QApplication>
#include <QTranslator>

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = 0);
    ~AboutDialog();

    QString currentLanguage;

    void switchLanguage();

private:
    Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_H
