#include "minescene.h"
QPoint t_around[8] =
{
    QPoint(-1,-1),
    QPoint(-1,0),
    QPoint(-1,1),
    QPoint(0,-1),
    QPoint(0,1),
    QPoint(1,-1),
    QPoint(1,0),
    QPoint(1,1)
};


void minescene::initscene()
{
    m_remainNoMines = m_scenecol * m_scenecol - m_minesum;        //剩余非雷数
    for(int i = 0; i < m_scenerow; i++)
    {
        QVector<mineitem *> tem;
        for(int j = 0; j < m_scenecol; j++)
        {
            mineitem *temp = new mineitem(i,j, QPixmap("://扫雷项目图片及声音资源/images/cover1.png"));
            temp->setPos(j * COMMON::MAPWIDTH, i * COMMON::MAPHEIGHT);
            this->addItem(temp);
            tem.push_back(temp);
        }
        m_itemVec.push_back(tem);
    }

    //随机布雷
    srand(time(NULL));
    for(int i = 0; i < m_minesum; i++)
    {
        int x = 0;
        int y = 0;
        x = rand() % m_scenerow;    // 产生一个坐标
        y = rand() % m_scenecol;
        if(!m_itemVec[x][y]->m_isMine)
        {
            m_itemVec[x][y]->m_isMine = true;
            this->countAroundMines(m_itemVec[x][y]);
        }
        else
            i--;
     }

    for(int i = 0; i < m_scenerow; i++)
    {
        for(int j = 0; j < m_scenecol; j++)
        {
            this->connect(this->m_itemVec[i][j], SIGNAL(sig_restartGame()), this, SIGNAL(sig_scenenewGame()));
            this->connect(this->m_itemVec[i][j], SIGNAL(sig_resetMineNum(int)), this, SIGNAL(sig_scenedisplayMineNum(int)));
        }
    }

}

void minescene::countAroundMines(mineitem *t_item)
{

    if(t_item == NULL || !t_item->m_isMine)
        return;
    for(int i = 0; i < 8; i++)
    {
        int x = t_item->m_X + t_around[i].x();
        int y = t_item->m_Y + t_around[i].y();
        if(x < 0 || x >= m_scenerow || y < 0 || y >= m_scenecol)
            continue;
        if(m_itemVec[x][y]->m_isMine)
            continue;
        m_itemVec[x][y]->m_aroundMineNum++;
    }
}

void minescene::openAllItems()
{
    if(this->m_isGameOver)
         return;
    for(int i = 0; i < m_scenerow; i++)
    {
        for(int j = 0; j < m_scenecol; j++)
        {
            m_itemVec[i][j]->m_isOpened = true;
            if(m_itemVec[i][j]->m_isMine)
            {
                m_itemVec[i][j]->setPixmap(QPixmap("://扫雷项目图片及声音资源/images/mine1.png"));
            }
            else
            {
                QString path = "://扫雷项目图片及声音资源/images/" + QString::number(m_itemVec[i][j]->m_aroundMineNum) + ".png";
                m_itemVec[i][j]->setPixmap(QPixmap(path));
            }
        }
    }
}

void minescene::expendWater(mineitem *t_item)
{
    if(t_item == NULL || t_item->m_isMine)  // 如果为空，或者为雷
        return;

    for(int i = 0; i < 8; i++)
    {
        int x = t_item->m_X + t_around[i].x();
        int y = t_item->m_Y + t_around[i].y();
        if(x < 0 || x >= m_scenerow || y < 0 || y >= m_scenecol)
            continue;
        else if(m_itemVec[x][y]->m_isMine || m_itemVec[x][y]->m_isOpened)
            continue;
        else if(m_itemVec[x][y]->m_rMouseKeyNum > 0)
            continue;

        m_itemVec[x][y]->m_isOpened = true;
        m_remainNoMines--;  // 非雷数减一
        int minenums = m_itemVec[x][y]->m_aroundMineNum;
        if(minenums == 0)
        {
            expendWater(m_itemVec[x][y]);
        }
        // 设置相应数字图片:
        QString path = "://扫雷项目图片及声音资源/images/" + QString::number(m_itemVec[x][y]->m_aroundMineNum) + ".png";
        m_itemVec[x][y]->setPixmap(QPixmap(path));

        if(m_remainNoMines == 0)    // 如果非雷数为0
        {
            m_isGameOver = true;
            if(m_soundOpen)
                QSound::play("://扫雷项目图片及声音资源/sound/win.wav");
            this->openAllItems();
            emit sig_successPassGame();
        }
    }
}

minescene::minescene(QObject *parent) : QGraphicsScene(parent)
{
    m_signedMineNum = 0;    //右键标记的雷数
    m_rightMineNum = 0;     //右键标记正确的雷数
    m_isGameOver = false;    // 设置游戏未结束
    m_soundOpen = true;      // 默认声音打开
}
