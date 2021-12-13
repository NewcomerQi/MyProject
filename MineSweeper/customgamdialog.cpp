#include "customgamdialog.h"

void CustomGamDialog::writesettings()
{
    QSettings *setting = new QSettings(tr("HQYJ"), tr("mine"), this);
    setting->beginGroup(tr("customGame"));
    setting->setValue(tr("custom_row"), rowspinbox->value());
    setting->setValue(tr("custom_col"), colspinbox->value());
    setting->setValue(tr("custom_mine"), minenumspinbox->value());
    setting->endGroup();
}

void CustomGamDialog::readsetting()
{
    QSettings *setting = new QSettings(tr("HQYJ"), tr("mine"), this);
    setting->beginGroup(tr("customGame"));
    int row = setting->value(tr("custom_row"), 10).toInt();
    int col = setting->value(tr("custom_col"), 10).toInt();
    int mine = setting->value(tr("custom_mine"), 11).toInt();
    setting->endGroup();

    rowspinbox->setValue(row);
    colspinbox->setValue(col);
    minenumspinbox->setValue(mine);
}

CustomGamDialog::CustomGamDialog(QWidget *parent) : QDialog(parent)
{
    this->setWindowTitle(tr("自定义游戏"));
    QLabel *label_1 = new QLabel(tr("行 数"));
    QLabel *label_2 = new QLabel(tr("列 数"));
    QLabel *label_3 = new QLabel(tr("雷 数"));


    colspinbox = new QSpinBox;
    colspinbox->setRange(1,100);
    colspinbox->setSingleStep(1);
    colspinbox->setValue(10);

    rowspinbox = new QSpinBox;
    rowspinbox->setRange(1,100);
    rowspinbox->setSingleStep(1);
    rowspinbox->setValue(10);

    minenumspinbox = new QSpinBox;
    minenumspinbox->setRange(1,100);
    minenumspinbox->setSingleStep(1);
    minenumspinbox->setValue(10);

    readsetting();
    QPushButton *ok_button=  new QPushButton(tr("确 定"));
    QPushButton *cancel_button = new QPushButton(tr("取 消"));

    QGridLayout * gLayout = new QGridLayout(this);
    gLayout->addWidget(label_1, 0,0);
    gLayout->addWidget(colspinbox, 0,1);
    gLayout->addWidget(label_2, 1,0);
    gLayout->addWidget(rowspinbox, 1,1);
    gLayout->addWidget(label_3, 2,0);
    gLayout->addWidget(minenumspinbox, 2,1);
    gLayout->addWidget(ok_button, 3,0);
    gLayout->addWidget(cancel_button,3,1);

    this->connect(ok_button, SIGNAL(clicked(bool)), this, SLOT(slot_accpetok()));
    this->connect(cancel_button, SIGNAL(clicked(bool)), this, SLOT(close()));
}

void CustomGamDialog::slot_accpetok()
{
    int row = rowspinbox->value();
    int column = colspinbox->value();
    int count = minenumspinbox->value();
    emit this->signal_sendCustomset(row, column, count);

    this->writesettings();
    this->close();
}
