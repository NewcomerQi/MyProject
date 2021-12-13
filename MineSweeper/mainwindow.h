#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QGraphicsView>
#include <QPushButton>
#include <QLCDNumber>
#include <QTime>
#include <QTimer>
#include <QActionGroup>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QSound>
#include <QInputDialog>
#include <QLineEdit>
#include <QDebug>
#include <QColorDialog>
#include "common.h"
#include "herodialog.h"
#include "customgamdialog.h"
#include "mineitem.h"
#include "minescene.h"

class Mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    QAction *newaction;
    QAction *lowleveaction;
    QAction *middleleveaction;
    QAction *heightleveaction;
    QAction *customaction;
    QAction *coloraction;
    QAction *soundaction;
    QAction *heroaction;    //英雄榜行为
    QAction *exitaction;
    QAction *aboutaction;

    // slot_newganme
    QWidget *centralget;    //主程序界面mainwindow->setcellwidget(centralget) slot_newganem
    QGraphicsView *mineView;    //主程序视图
    minescene *MineScene;       //主程序场景
    QLCDNumber *minenumLcd;     //雷数LCD
    QLCDNumber *timeLcd;        //时间LCD
    QPushButton *smilebtn;      //笑脸按钮
    int sound;

    int m_time;                 //当前用时数
    QTimer *m_timer;            //定时器

    void create_action();       //创建行为
    void create_menu();     //创建菜单
    void readsettings();    //读当前游戏设置
    void writesettings();       //写当前游戏设置

    Mainwindow(QWidget *parent = 0);
    ~Mainwindow();

signals:

public slots:
    void slot_newgame();        //开始新游戏槽
    void slot_newgamebyleve(QAction *);     //点击游戏级别行为开始新游戏槽
    void slot_acceptCutsomvale(int,int,int);    //接收自定义游戏设置槽
    void slot_colorchanged();       //设置颜色的槽
    void slot_soundchanged();       //设置声音的槽
    void slot_herochecked();        //显示英雄榜的槽
    void slot_about();              //显示about扫雷的槽
    void slot_updatehero();         //更新英雄榜的槽
    void slot_displayMineNum(int);  //显示雷数的槽
    void slot_displayTime();            //显示时间的槽
};

#endif // MAINWINDOW_H
