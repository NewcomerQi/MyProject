#ifndef CUSTOMGAMDIALOG_H
#define CUSTOMGAMDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QSpinBox>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QSettings>

class CustomGamDialog : public QDialog
{
    Q_OBJECT
public:
    QSpinBox *colspinbox;
    QSpinBox *rowspinbox;
    QSpinBox *minenumspinbox;
    void readsetting();
    void writesettings();

    explicit CustomGamDialog(QWidget *parent = 0);

signals:
    void signal_sendCustomset(int, int, int);

public slots:
    void slot_accpetok();
};

#endif // CUSTOMGAMDIALOG_H
