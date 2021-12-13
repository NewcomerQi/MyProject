#ifndef HERODIALOG_H
#define HERODIALOG_H

#include <QWidget>
#include <QLabel>
#include <QDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QSettings>

class herodialog : public QDialog
{
    Q_OBJECT
public:
    QLabel *lowrecordtimelabel;     //初级纪录时间
    QLabel *middlerecordtimelabel;  //中级纪录时间
    QLabel *heightrecordtimelabel;  //高级纪录时间
    QLabel *lowrecordnamelabel;     //初级纪录姓名
    QLabel *middlerecordnamelabel;  //中级纪录姓名
    QLabel *heightrecordnamelabel;  //高级纪录姓名
    void readsettings();    //读英雄榜数据
    void init_herorecord(); //初始化英雄榜数据

    explicit herodialog(QWidget *parent = 0);

signals:

public slots:
    void slot_reset();  //重新设置英雄榜数据槽(slots:)
};

#endif // HERODIALOG_H
