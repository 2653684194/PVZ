#ifndef PLANT_HPP
#define PLANT_HPP

#include "Instances.hpp"

template class BulletPool<Bullet>;

template <class T>
bool faceZombie(ZombiePool<T>& ZP,Plant& pl){
    short x=pl.getx();
    auto node=ZP.Head;
    while(node&&node->item->getx()<x){
        // std::cout<<node->item->getx()<<" Plant:"<<x<<"\n";
        node=node->next;
    }
    if (!node)
        return false;
    return true;
}

class SunFlower:public Plant{
    TICK ActionInterval;//将来可能叠加buff效果，考虑外加变量或者取消static设定
    
    static short SunAmount;//每次生成的阳光数量
public:
    const static COST cost;
    const static TICK CoolDown;

    static float SizeFactor;
    static std::string ImgPath;
    static short frame;
    static short FrameStartInd;
    /*考虑形态进化，需要使用ImgPath2和frame2，另外使用StatusFlag标记*/
    SunFlower(COORD xx=0,COORD yy=0);
    void show(sf::RenderWindow& wd);
    void action();
    //必须声明为static
    static void LoadTexture();

    COST getCost()const;
    TICK getCoolDown()const;
    
    short getFrameStartInd()const;

    bool SaveInstance();
    bool LoadInstance();
};

class PeaShooter:public Plant{
protected:
    static TICK ActionInterval;//每次发射豌豆的时间间隔
    static DG dg;
    
    // short x,y;
    // TICK TickGap;
    // short i;//
    uint8_t PeaFlag;// 1:普通豌豆 2:冰豌豆
public:
    const static COST cost;// PeaShooter的消耗阳光
    const static TICK CoolDown;// PeaShooter的冷却时间

    static float SizeFactor;// PeaShooter的大小因子

    static std::string ImgPath;
    static short frame;// PeaShooter的帧数
    static short FrameStartInd;// PeaShooter的帧起始索引


    PeaShooter(COORD xx=0,COORD yy=0);
    void show(sf::RenderWindow& wd);
    void action();

    static void LoadTexture();

    COST getCost()const;
    TICK getCoolDown()const;

    short getFrameStartInd()const;

    bool SaveInstance();
    bool LoadInstance();
};

class SnowPeaShooter : public PeaShooter{
    
    static DG dg;
public:
    const static COST cost;
    const static TICK CoolDown;

    static float SizeFactor;

    static std::string ImgPath;
    static short frame;
    static short FrameStartInd;

    SnowPeaShooter(COORD xx = 0, COORD yy = 0);

    static void LoadTexture();

    void show(sf::RenderWindow& wd) override;
    // void action() override;

    COST getCost() const override;
    TICK getCoolDown() const override;
    short getFrameStartInd() const override;

    bool SaveInstance() override;
    bool LoadInstance() override;
};


class Repeater : public PeaShooter{
    
    const static TICK TwoPeasInterval;
    TICK PeasTickGap;

public:
    const static COST cost;
    const static TICK CoolDown;

    static float SizeFactor;
    static std::string ImgPath;
    static short frame;
    static short FrameStartInd;

    Repeater(COORD xx = 0, COORD yy = 0);
    
    static void LoadTexture();
    void show(sf::RenderWindow& wd) override;
    void action() override;

    COST getCost() const override;
    TICK getCoolDown() const override;
    short getFrameStartInd() const override;

    bool SaveInstance() override;
    bool LoadInstance() override;
};

class WallNut : public Plant {
    
    // static TICK ActionInterval; //它什么操作都不做
public:
    static float SizeFactor;
    static std::string NormalImgPath;
    static short NormalFrame;
    static short NormalFrameStartInd;
    static std::string Eaten1ImgPath;
    static short Eaten1Frame;
    static short Eaten1FrameStartInd;
    static std::string Eaten2ImgPath;
    static short Eaten2Frame;
    static short Eaten2FrameStartInd;

    const static COST cost;
    const static TICK CoolDown;

    WallNut(COORD xx = 0, COORD yy = 0);
    void show(sf::RenderWindow& wd) override;
    void action() override; //它什么都不做
    static void LoadTexture();


    COST getCost() const override;
    TICK getCoolDown() const override;
    short getFrameStartInd() const override;
    
    bool SaveInstance()override;
    bool LoadInstance()override;
};

class TallNut : public Plant {
public:
    static float SizeFactor;
    // static TICK ActionInterval; //它什么操作都不做
    static std::string ImgPath;
    static short frame;
    static short FrameStartInd;

    const static COST cost;
    const static TICK CoolDown;

    TallNut(COORD xx = 0, COORD yy = 0);
    void show(sf::RenderWindow& wd) override;
    void action() override {} //它什么都不做
    static void LoadTexture();

    COST getCost() const override { return cost; }
    TICK getCoolDown() const override { return CoolDown; }
    short getFrameStartInd() const override { return FrameStartInd; }
};


#endif