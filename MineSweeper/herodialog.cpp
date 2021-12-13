#include "herodialog.h"

void herodialog::readsettings()
{
    QSettings *setting = new QSettings(tr("HQYJ"), tr("mine"), this);
    setting->beginGroup(tr("herorecord"));
    QString lowertime = setting->value(tr("lowrecordtime"), "999").toString();
    QString middletime = setting->value(tr("middlerecordtime"), "999").toString();
    QString heighttime = setting->value(tr("heightrecordtime"), "999").toString();

    QString lowername = setting->value("lowrecordname", "匿名").toString();
    QString middlename = setting->value("middlerecordname", "匿名").toString();
    QString heightname = setting->value("heightrecordname", "匿名").toString();
    setting->endGroup();

    lowrecordtimelabel->setText(lowertime);
    middlerecordtimelabel->setText(middletime);
    heightrecordtimelabel->setText(heighttime);

    lowrecordnamelabel->setText(lowername);
    middlerecordnamelabel->setText(middlename);
    heightrecordnamelabel->setText(heightname);
}

void herodialog::init_herorecord()
{
    QSettings *setting = new QSettings("HQYJ", "mine", this);
    setting->beginGroup("herorecord");
    setting->setValue("lowrecordtime", lowrecordtimelabel->text());
    setting->setValue("middlerecordtime", middlerecordtimelabel->text());
    setting->setValue("heightrecordtime", heightrecordtimelabel->text());

    setting->setValue("lowrecordname", lowrecordnamelabel->text());
    setting->setValue("middlerecordname",middlerecordnamelabel->text());
    setting->setValue("heightrecordname", middlerecordnamelabel->text());

    setting->endGroup();
}

herodialog::herodialog(QWidget *parent) : QDialog(parent)
{

    this->setWindowTitle(tr("扫雷英雄榜"));
    this->setWindowIcon(QIcon("://扫雷项目图片及声音资源/images/minetip1.png"));
    QLabel * lower_label = new QLabel(tr("初级"));
    QLabel * mid_label = new QLabel(tr("中级"));
    QLabel * height_label = new QLabel(tr("高级"));
    lowrecordtimelabel = new QLabel;    //初级纪录时间
    middlerecordtimelabel = new QLabel;  //中级纪录时间
    heightrecordtimelabel = new QLabel;  //高级纪录时间
    lowrecordnamelabel = new QLabel;     //初级纪录姓名
    middlerecordnamelabel = new QLabel;  //中级纪录姓名
    heightrecordnamelabel = new QLabel;  //高级纪录姓名
    readsettings();
    QPushButton *reset_button = new QPushButton(tr("重新设置"));
    this->connect(reset_button, SIGNAL(clicked(bool)), this, SLOT(slot_reset()));
    QPushButton *ok_button = new QPushButton(tr("确定"));
    this->connect(ok_button, SIGNAL(clicked(bool)), this, SLOT(close()));

    QGridLayout *gLayout = new QGridLayout(this);
    gLayout->addWidget(lower_label, 0,0);
    gLayout->addWidget(lowrecordtimelabel,0,1);
    gLayout->addWidget(lowrecordnamelabel,0,2);

    gLayout->addWidget(mid_label, 1,0);
    gLayout->addWidget(middlerecordtimelabel,1,1);
    gLayout->addWidget(middlerecordnamelabel,1,2);

    gLayout->addWidget(height_label,2,0);
    gLayout->addWidget(heightrecordtimelabel,2,1);
    gLayout->addWidget(heightrecordnamelabel,2,2);

    QHBoxLayout * hLayout = new QHBoxLayout;
    hLayout->addWidget(reset_button);
    hLayout->addWidget(ok_button);
    gLayout->addLayout(hLayout, 3,0,1,2);
    init_herorecord();
}

void herodialog::slot_reset()
{
    lowrecordtimelabel->setText("999");    //初级纪录时间
    middlerecordtimelabel->setText("999");  //中级纪录时间
    heightrecordtimelabel->setText("999");  //高级纪录时间
    lowrecordnamelabel->setText(tr("匿名"));     //初级纪录姓名
    middlerecordnamelabel->setText(tr("匿名"));  //中级纪录姓名
    heightrecordnamelabel->setText(tr("匿名"));  //高级纪录姓名
    init_herorecord();
}
