#ifndef ZOMBIE_HPP
#define ZOMBIE_HPP

#include "Instances.hpp"
template class Map<Plant>;//因为Map是模板而没声明时立刻定义，所以要显示实例化
template class ZombiePool<Zombie>;




inline HP* Zombie::GetPlantHP(Plant* plant){//可能是plant和zombie产生耦合，导致zombie隐式include，使得showauxiliary重定义
    return &(plant->hp);
}

class RegularZombie:public Zombie{
    const static COORD ImgInc;//图片偏移量，单位为像素

    static float SizeFactor;

    static TICK ActionInterval;
    static DG dg;
    static SPD speed;
    
    bool isstop(){return i==8||i==9||i==7||i==1||i==2||i==3||i==17||i==0;}//通过动画帧判断僵尸是否停止移动

public:
    static std::string DieImgPath;
    static short DieFrameStartIndex;
    static short DieFrame;
    static std::string EatImgPath;
    static short EatFrameStartIndex;
    static short EatFrame;
    static std::string WalkImgPath;
    static short WalkFrameStartIndex;
    static short WalkFrame;

    RegularZombie(COORD xx=500,COORD yy=0);
    void walk(RWD& wd);
    void eat(RWD& wd,Plant*);
    void update(RWD& wd);
    void LoadTexture();
    bool die(RWD& wd);
    COORD getx();//获取僵尸的x坐标，单位为像素
    COORD getGridx();//获取僵尸所在格子的x坐标

    bool SaveInstance();
    bool LoadInstance();
};

class ConeheadZombie : public RegularZombie {
    const static COORD ImgInc;

    static float SizeFactor;

    static TICK ActionInterval;
    static DG dg;
    static SPD speed;

    bool isstop() {
        return i == 8 || i == 9 || i == 7 || i == 1 || i == 2 || i == 3 || i == 17 || i == 0;
    }

public:
    static std::string DieImgPath;
    static short DieFrameStartIndex;
    static short DieFrame;
    static std::string EatImgPath;
    static short EatFrameStartIndex;
    static short EatFrame;
    static std::string WalkImgPath;
    static short WalkFrameStartIndex;
    static short WalkFrame;

    ConeheadZombie(COORD xx = 500, COORD yy = 0);
    void walk(RWD& wd);
    void eat(RWD& wd, Plant*);
    void update(RWD& wd);
    void LoadTexture();
    bool die(RWD& wd);
    COORD getx();
    COORD getGridx();

    bool SaveInstance();
    bool LoadInstance();
};


#endif