#ifndef COMMON_H
#define COMMON_H

class COMMON
{
public:
    // 游戏级别定义
    enum gameLeve
    {
        LOWLEVE = 0,
        MIDDLELEVE = 1,
        HEIGHTLEVE = 2,
        CUSOMLEVE = 3
    };

    // 初始行，列，雷数设置
    enum lowset
    {
        LOWROWANDCOL = 9,
        LOWMINENUM = 10
    };

    // 中级行，列，雷数设置
    enum middleset
    {
        MIDDLEROWANDCOL = 16,
        MIDDLEMINENUM = 40
    };

    // 高级行，列，雷数设置
    enum heightset
    {
        HEIGHTROW = 16,
        HEIGHTCOL = 30,
        HEIGHTMINENUM = 99
    };

    // 图片大小设置
    enum mapset
    {
        MAPWIDTH = 25,
        MAPHEIGHT = 25
    };
};

#endif // COMMON_H

