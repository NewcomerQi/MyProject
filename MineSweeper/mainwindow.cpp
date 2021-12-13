#include "mainwindow.h"

void Mainwindow::create_action()
{
    newaction = new QAction(tr("开局(&N)"), this);
    this->connect(newaction, SIGNAL(triggered(bool)), this, SLOT(slot_newgame()));
    newaction->setShortcut(tr("N"));

    lowleveaction = new QAction(tr("初级(&B)"),this);   // 初级
    lowleveaction->setCheckable(true);
    lowleveaction->setShortcut(tr("B"));

    middleleveaction = new QAction(tr("中级(&I)"), this);    // 中级
    middleleveaction->setCheckable(true);
    middleleveaction->setShortcut(tr("I"));

    heightleveaction = new QAction(tr("高级(&E)"), this);    // 高级
    heightleveaction->setCheckable(true);
    heightleveaction->setShortcut(tr("E"));

    customaction = new QAction(tr("自定义(&C)"), this);       // 自定义
    customaction->setCheckable(true);
    customaction->setShortcut(tr("C"));

    coloraction = new QAction(tr("颜色(&L)"), this);     // 颜色
    coloraction->setShortcut(tr("L"));
    this->connect(coloraction, SIGNAL(triggered(bool)), this, SLOT(slot_colorchanged()));

    soundaction = new QAction(tr("声音(&S)"), this);     // 声音
    this->connect(soundaction, SIGNAL(triggered(bool)), this, SLOT(slot_soundchanged()));
    soundaction->setShortcut(tr("S"));
    soundaction->setChecked(true);
    soundaction->setCheckable(true);

    heroaction = new QAction(tr("扫雷英雄榜(&T)"), this);    //英雄榜行为
    heroaction->setShortcut(tr("T"));
    this->connect(heroaction, SIGNAL(triggered(bool)), this, SLOT(slot_herochecked()));

    exitaction = new QAction(tr("退出(&X)"), this);  // 退出
    exitaction->setShortcut(tr("X"));
    this->connect(exitaction, SIGNAL(triggered(bool)), this, SLOT(close()));

    aboutaction = new QAction(tr("关于扫雷(&A)"), this);  // 关于扫雷
    aboutaction->setShortcut(tr("A"));
    this->connect(aboutaction, SIGNAL(triggered(bool)), this, SLOT(slot_about()));

    // 添加行为组
    QActionGroup * action_group = new QActionGroup(this);
    action_group->addAction(lowleveaction);
    action_group->addAction(middleleveaction);
    action_group->addAction(heightleveaction);
    action_group->addAction(customaction);

    // 连接信号号行为组槽函数
    this->connect(action_group, SIGNAL(triggered(QAction* )), this, SLOT(slot_newgamebyleve(QAction* )));

}

void Mainwindow::create_menu()
{
    QMenuBar *mbar = this->menuBar();
    // 添加两个菜单,游戏和帮助
    QMenu *game_menu = mbar->addMenu(tr("游戏(&G)"));
    game_menu->addAction(newaction);    // 开局
    game_menu->addSeparator();  // 设置分割线


    game_menu->addAction(lowleveaction);    // 初级
    game_menu->addAction(middleleveaction);    // 中级
    game_menu->addAction(heightleveaction); // 高级
    game_menu->addAction(customaction);     // 自定义
    game_menu->addSeparator();  // 设置分割线

    game_menu->addAction(coloraction);  // 颜色
    game_menu->addAction(soundaction);  // 声音
    game_menu->addSeparator();  // 设置分割线

    game_menu->addAction(heroaction);   // 扫雷英雄榜
    game_menu->addSeparator();  // 设置分割线

    game_menu->addAction(exitaction);   // 退出

    // 帮助菜单
    QMenu *help_menu = mbar->addMenu(tr("帮助(&H)"));
    help_menu->addAction(aboutaction);  // 关于
    aboutaction->setShortcut(tr("H")); // 设置快捷键
}
// 读取注册表
void Mainwindow::readsettings()
{
    QSettings *setting = new QSettings(tr("HQYJ"), tr("mine"), this);
    setting->beginGroup(tr("customGame"));
    int row = setting->value(tr("row"), 10).toInt();
    int col = setting->value(tr("col"), 10).toInt();
    int mine = setting->value(tr("mine"), 11).toInt();
    int sounds = setting->value(tr("sound"), 1).toInt();    // 读取声音是否开
    setting->endGroup();

    this->MineScene->m_scenerow = row;
    this->MineScene->m_scenecol = col;
    this->MineScene->m_minesum = mine;
    this->sound = sounds;
    if(row == COMMON::LOWROWANDCOL && col == COMMON::LOWROWANDCOL && mine == COMMON::LOWMINENUM)
        this->MineScene->m_crrentleve = COMMON::LOWLEVE;
    else if(row == COMMON::MIDDLEROWANDCOL && col == COMMON::MIDDLEROWANDCOL && mine == COMMON::MIDDLEMINENUM)
        this->MineScene->m_crrentleve = COMMON::MIDDLELEVE;
    else if(row == COMMON::HEIGHTROW && col == COMMON::HEIGHTCOL && mine == COMMON::HEIGHTMINENUM)
        this->MineScene->m_crrentleve = COMMON::HEIGHTLEVE;
    else
        this->MineScene->m_crrentleve = COMMON::CUSOMLEVE;
}

// 写入注册表
void Mainwindow::writesettings()
{
    QSettings *setting = new QSettings(tr("HQYJ"), tr("mine"), this);
    setting->beginGroup(tr("customGame"));
    setting->setValue(tr("row"), this->MineScene->m_scenerow);
    setting->setValue(tr("col"), this->MineScene->m_scenecol);
    setting->setValue(tr("mine"), this->MineScene->m_minesum);
    setting->setValue(tr("sound"),this->sound); // 存入声音
    setting->endGroup();
}

Mainwindow::Mainwindow(QWidget *parent)
    : QMainWindow(parent), centralget(0),mineView(0),MineScene(0),minenumLcd(0),timeLcd(0),smilebtn(0),m_timer(0)
{
    this->setWindowIcon(QIcon("://扫雷项目图片及声音资源/images/minetip1.png"));   // 扫雷游戏图标
    this->setWindowTitle(tr("扫雷"));
    this->setWindowFlags(Qt::WindowCloseButtonHint);

    this->create_action();  //创建行为
    this->create_menu();    //创建菜单
    this->slot_newgame();   // 开始游戏
}

Mainwindow::~Mainwindow()
{

}

void Mainwindow::slot_newgame()
{
    // 清理对象
    centralget->deleteLater();
    mineView->deleteLater();
    MineScene->deleteLater();
    minenumLcd->deleteLater();
    timeLcd->deleteLater();
    smilebtn->deleteLater();
    m_timer->deleteLater();

    centralget = new QWidget(this);     // 主界面对象

    // 创建调色板，雷数LCD，时间LCD，颜色
    QPalette textPalette;
    textPalette.setColor(QPalette::Normal, QPalette::WindowText, Qt::red);
    minenumLcd = new QLCDNumber(centralget);

    minenumLcd->setPalette(textPalette);
    minenumLcd->setDigitCount(3);

    timeLcd = new QLCDNumber(centralget);
    timeLcd->setPalette(textPalette);
    timeLcd->setDigitCount(3);

    smilebtn = new QPushButton;      //笑脸按钮
    smilebtn->setIcon(QIcon("://扫雷项目图片及声音资源/images/cenWin0.png"));  // 笑脸图标
    this->connect(smilebtn, SIGNAL(clicked(bool)), this, SLOT(slot_newgame()));

    QVBoxLayout * vLayout = new QVBoxLayout(centralget);  // 指定父类对象
    QHBoxLayout * hLayout = new QHBoxLayout;

    // 添加水平布局
    hLayout->addWidget(minenumLcd);
    hLayout->addStretch();
    hLayout->addWidget(smilebtn);
    hLayout->addStretch();
    hLayout->addWidget(timeLcd);

    // 视图
    mineView = new QGraphicsView(centralget);    //主程序视图
    MineScene = new minescene(mineView);     //主程序场景
    mineView->setScene(MineScene);  //把场景添加到视图
    this->readsettings();
    if(sound == 1)  // 如果读取到注册表的声音为1，则打开
    {
        soundaction->setChecked(true);
        MineScene->m_soundOpen = true;
    }
    else
    {
        soundaction->setChecked(false); // 否则关闭
        MineScene->m_soundOpen = false;
    }

    // 将场景的开始新信号连接到开始新游戏槽，显示雷数的槽，
    this->connect(MineScene, SIGNAL(sig_scenenewGame()), this, SLOT(slot_newgame()));
    this->connect(MineScene, SIGNAL(sig_scenedisplayMineNum(int)), this, SLOT(slot_displayMineNum(int)));
    this->setCentralWidget(centralget);

    // 根据图片大小和行，列数据设置场景，主视图主窗口大小
    MineScene->setSceneRect(0,0,MineScene->m_scenecol * COMMON::MAPWIDTH, MineScene->m_scenerow * COMMON::MAPHEIGHT);
    mineView->setFixedSize(MineScene->m_scenecol * COMMON::MAPWIDTH + 5, MineScene->m_scenerow * COMMON::MAPHEIGHT + 5);
    this->setFixedSize(MineScene->m_scenecol * COMMON::MAPWIDTH + 25, MineScene->m_scenerow * COMMON::MAPHEIGHT + 100);
    vLayout->addLayout(hLayout);
    vLayout->addWidget(mineView);

    // 根据当前场景的游戏级别设置相应的级别行为被选中
    if(MineScene->m_crrentleve == COMMON::LOWLEVE)
        lowleveaction->setChecked(true);
    else if(MineScene->m_crrentleve == COMMON::MIDDLELEVE)
        middleleveaction->setChecked(true);
    else if(MineScene->m_crrentleve == COMMON::HEIGHTLEVE)
        heightleveaction->setChecked(true);
    else
        customaction->setChecked(true);
    this->writesettings();
    // LCD显示雷数和时间(初始为0)
    m_time = 0;
    minenumLcd->display(MineScene->m_minesum);
    timeLcd->display(m_time);

    // 创建定时器，将超时信号连接到显示时间槽
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(slot_displayTime()));
    m_timer->start(1000);

    // 初始化场景，启动定时器
    MineScene->initscene();

    this->connect(MineScene, SIGNAL(sig_successPassGame()), this, SLOT(slot_updatehero()));

}

void Mainwindow::slot_newgamebyleve(QAction * act)
{
    if(act == lowleveaction)
    {
        this->MineScene->m_scenerow = COMMON::LOWROWANDCOL;
        this->MineScene->m_scenecol = COMMON::LOWROWANDCOL;
        this->MineScene->m_minesum = COMMON::LOWMINENUM;
        this->MineScene->m_crrentleve = COMMON::LOWLEVE;
        lowleveaction->setChecked(true);
        this->writesettings();
    }
    else if(act == middleleveaction)
    {
        this->MineScene->m_scenerow = COMMON::MIDDLEROWANDCOL;
        this->MineScene->m_scenecol = COMMON::MIDDLEROWANDCOL;
        this->MineScene->m_minesum = COMMON::MIDDLEMINENUM;
        this->MineScene->m_crrentleve = COMMON::MIDDLELEVE;
        middleleveaction->setChecked(true);
        this->writesettings();
    }
    else if(act == heightleveaction)
    {
        this->MineScene->m_scenerow = COMMON::HEIGHTROW;
        this->MineScene->m_scenecol = COMMON::HEIGHTCOL;
        this->MineScene->m_minesum = COMMON::HEIGHTMINENUM;
        this->MineScene->m_crrentleve = COMMON::HEIGHTLEVE;
        heightleveaction->setChecked(true);
        this->writesettings();
    }
    else    // 自定义
    {
        CustomGamDialog *custom = new CustomGamDialog(this);
        this->connect(custom, SIGNAL(signal_sendCustomset(int,int,int)), this, SLOT(slot_acceptCutsomvale(int,int,int))); // 连接自定义槽函数
        customaction->setChecked(true);
        custom->exec();
    }
    // 开始新游戏
    this->slot_newgame();
}

void Mainwindow::slot_acceptCutsomvale(int row, int col, int mine)
{
    MineScene->m_crrentleve = COMMON::CUSOMLEVE;
    MineScene->m_scenerow = row;
    MineScene->m_scenecol = col;
    MineScene->m_minesum = mine;
    this->writesettings();
    // 将自定义游戏数据写入到注册表的自定义游戏设置中
    this->slot_newgame();
}

void Mainwindow::slot_colorchanged()
{
    QPalette pale = this->palette();
    QColor color = QColorDialog::getColor(Qt::white, this, tr("请选择颜色"));
    if(color.isValid())
    {
        pale.setColor(QPalette::Window,color);
        pale.setColor(QPalette::WindowText,color);
    }
    this->setPalette(pale);
}

void Mainwindow::slot_soundchanged()
{
    if(!soundaction->isChecked())
    {
        soundaction->setChecked(false);
        MineScene->m_soundOpen = false;
        sound = 0;
    }
    else
    {
        soundaction->setChecked(true);
        MineScene->m_soundOpen = true;
        sound = 1;
    }
    writesettings();
    this->update();     // 程序更新
}

void Mainwindow::slot_herochecked()
{
    // 创建英雄榜对象,并显示
    herodialog HeroDialog(this);
    HeroDialog.exec();
}

void Mainwindow::slot_about()
{
    QMessageBox::about(this, tr("关于扫雷"), tr("游戏名称: 扫雷\n游戏版本: 1.0\n游戏作者: HYQ\n发布日期: 2021-8-21"));
}

// 更新英雄榜槽函数
void Mainwindow::slot_updatehero()
{
    QSettings *setting = new QSettings(tr("HQYJ"), tr("mine"), this);
    int recordtime = 0;
    QString newname;

    switch (MineScene->m_crrentleve)
    {
    case COMMON::LOWLEVE:   // 初级
        setting->beginGroup(tr("herorecord"));
        recordtime = setting->value(tr("lowrecordtime")).toInt();
        setting->endGroup();
        if(m_time < recordtime) // 判断时间是否更短
        {
            bool ok;
            newname = QInputDialog::getText(this, tr("新纪录"), tr("新纪录!，请输入你的名字: "), QLineEdit::Normal, tr("匿名"), &ok);
            if(ok && !newname.trimmed().isEmpty())  // 不为空
            {
                setting->beginGroup(tr("herorecord"));  // 写入英雄榜
                setting->setValue(tr("lowrecordtime"), QString::number(m_time));
                setting->setValue(tr("lowrecordname"), newname);
                setting->endGroup();
            }
        }
        break;
    case COMMON::MIDDLELEVE:    // 中级
        setting->beginGroup(tr("herorecord"));
        recordtime = setting->value(tr("middlerecordtime")).toInt();
        setting->endGroup();
        if(m_time < recordtime)
        {
            bool ok;
            newname = QInputDialog::getText(this, tr("新纪录"), tr("新纪录!，请输入你的名字: "), QLineEdit::Normal, tr("匿名"), &ok);
            if(ok && !newname.trimmed().isEmpty())
            {
                setting->beginGroup(tr("herorecord"));
                setting->setValue(tr("middlerecordtime"), QString::number(m_time));
                setting->setValue(tr("middlerecordname"), newname);
                setting->endGroup();
            }
        }
        break;
    case COMMON::HEIGHTLEVE:    // 高级
        setting->beginGroup(tr("herorecord"));
        recordtime = setting->value(tr("heightrecordtime")).toInt();
        setting->endGroup();
        if(m_time < recordtime)
        {
            bool ok;
            newname = QInputDialog::getText(this, tr("新纪录"), tr("新纪录!，请输入你的名字: "), QLineEdit::Normal, tr("匿名"), &ok);
            if(ok && !newname.trimmed().isEmpty())
            {
                setting->beginGroup(tr("herorecord"));
                setting->setValue(tr("heightrecordtime"), QString::number(m_time));
                setting->setValue(tr("heightrecordname"), newname);
                setting->endGroup();
            }
        }
        break;
    }
    this->slot_herochecked();
    int result = QMessageBox::question(this, tr("游戏结束"), tr("是否重新开始游戏"), QMessageBox::Yes | QMessageBox::No);
    if(result == QMessageBox::Yes)
        this->slot_newgame();
    else if(result == QMessageBox::No)
        this->close();
}


void Mainwindow::slot_displayMineNum(int minenums)
{
    int mine_nums = MineScene->m_minesum-minenums;
    if(mine_nums < 0)
       mine_nums = 0;
    minenumLcd->display(mine_nums);
}

void Mainwindow::slot_displayTime()
{
    if(!MineScene->m_isGameOver)
    {
        m_time++;
        timeLcd->display(m_time);
        if(MineScene->m_soundOpen)
        {
            QSound::play("://扫雷项目图片及声音资源/sound/time.wav");
        }
    }
    else
        m_timer->stop();
}




