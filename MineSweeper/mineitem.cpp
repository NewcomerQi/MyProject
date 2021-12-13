#include "mineitem.h"
#include "minescene.h"

void mineitem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // 获取场景
    minescene * MineScene = (minescene *)this->scene();

    // 如果游戏结束或者方块已经打开
    if(MineScene->m_isGameOver || m_isOpened)
    {
        return;
    }
    if(event->button() == Qt::LeftButton && !m_isRightMouse)    // 如果是左键，设置图片
    {
        // 设置图片
        this->setPixmap(QPixmap("://扫雷项目图片及声音资源/images/mine1_0.png"));
    }
    else if(event->button() == Qt::RightButton)
    {
        m_isRightMouse = true;
        m_rMouseKeyNum++;
        if(m_rMouseKeyNum == 1)
        {
            m_isRightMouse = true;
            MineScene->m_signedMineNum++;
            this->setPixmap(QPixmap("://扫雷项目图片及声音资源/images/flag1.png"));   // 棋子
            emit sig_resetMineNum(MineScene->m_signedMineNum);
            if(this->m_isMine)
                MineScene->m_rightMineNum++;
        }
        else if(m_rMouseKeyNum == 2)
        {
            m_isRightMouse = true;
            MineScene->m_signedMineNum--;
            this->setPixmap(QPixmap("://扫雷项目图片及声音资源/images/interrogation1_0.png"));   // 设置问号
            emit sig_resetMineNum(MineScene->m_signedMineNum);

            if(this->m_isMine)
                MineScene->m_rightMineNum--;
        }
        else if(m_rMouseKeyNum == 3)
        {
            m_isRightMouse = false;
            this->setPixmap(QPixmap("://扫雷项目图片及声音资源/images/cover1.png"));   // 设置初始方块
            m_rMouseKeyNum = 0;
        }
    }
}

void mineitem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    minescene *MineScene = (minescene *)this->scene();  // 设置场景

    if(MineScene->m_isGameOver || m_isOpened || m_isRightMouse)
        return;

    if(event->button() == Qt::LeftButton)
    {
        if(this->m_rMouseKeyNum == 1)
        {
            return;
        }
        m_isOpened = true;

        if(this->m_isMine)    // 如果是雷
        {
            this->setPixmap(QPixmap("://扫雷项目图片及声音资源/images/mine1.png"));   // 设置雷的图片
            if(MineScene->m_soundOpen)
            {
                QSound::play("://扫雷项目图片及声音资源/sound/bong.wav");   // 播放雷的声音
            }
            MineScene->openAllItems();
            MineScene->m_isGameOver = true;
            QMessageBox::information((QWidget *)MineScene->parent(),tr("扫雷失败"),tr("扫雷结束"), QMessageBox::Ok);
            emit this->sig_restartGame();
            return;
        }
        else    // 如果不是雷
        {
            if(!this->m_aroundMineNum)
            {
                this->setPixmap(QPixmap("://扫雷项目图片及声音资源/images/0.png"));   // 设置图片
                MineScene->expendWater(this);
            }
            else    // 如果周围雷数不为0则设置方块图片为方块中周围雷数所对应的图片
            {
                QString path = "://扫雷项目图片及声音资源/images/" + QString::number(m_aroundMineNum) + ".png";
                this->setPixmap(QPixmap(path));
            }

        }

        // 将场景中非雷数减1
        MineScene->m_remainNoMines--;
        if(!MineScene->m_remainNoMines) // 如果非雷数为0，游戏结束
        {
            MineScene->m_isGameOver = true;
            if(MineScene->m_soundOpen)
            {
                QSound::play("://扫雷项目图片及声音资源/sound/win.wav");   //
            }
            MineScene->openAllItems();
            MineScene->sig_successPassGame();
        }
    }
}

mineitem::mineitem(int t_x, int t_y, QPixmap pixmap, QGraphicsItem *parent) : QGraphicsPixmapItem(pixmap)
{
    m_X = t_x;
    m_Y = t_y;
    m_isOpened = 0;
    m_aroundMineNum = 0;
    m_isMine = false;
    m_rMouseKeyNum = 0;
    m_isRightMouse = false;
}
