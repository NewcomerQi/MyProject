#ifndef MINEITEM_H
#define MINEITEM_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QDebug>
#include <QMessageBox>

class mineitem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    int  m_X;         //在容器中的行数
    int  m_Y;         //在容器中的列数
    bool m_isMine;    //是否为雷
    bool m_isOpened;  //是否打开
    int  m_aroundMineNum; //周围雷数
    int  m_rMouseKeyNum;  //右键点击的次数
    bool m_isRightMouse;    // 右键是否标记

    void mousePressEvent(QGraphicsSceneMouseEvent *event);      //鼠标点击事件(右键)
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);  //鼠标释放事件（左键）


    explicit mineitem(int t_x, int t_y,QPixmap pixmap, QGraphicsItem *parent = NULL);

signals:
    void sig_restartGame();   //重新开始游戏信号
    void sig_resetMineNum(int t_signedMineNum);  //重设雷数信号

public slots:
};

#endif // MINEITEM_H
