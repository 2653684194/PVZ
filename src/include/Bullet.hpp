#ifndef BULLET_HPP
#define BULLET_HPP

#include "Instances.hpp"
// bool Bullet::isCollide(){
//     if (BulletPool[y].getFront()==this){




//PeaFlag:icy:2,normal:1,fire:0
class Pea:public Bullet{
protected:
    static std::string ImgPath;//把三种状态放在一个文件夹,通过flag控制
    static sf::Texture TextureArr[3];
    static float SizeFactor;
    // static sf::Sprite* SParr[3];
    // static COORD ImgInc;
    
    uint8_t PeaFlag;//使用转换
public:
    

    Pea(COORD xx=0,COORD yy=0,SPD spd=10,DG dg=10,short flag=1);
    // ~Pea(){std::cout<<"Pea deleted\n";}
    void LoadTexture();
    void move(RWD& wd);
    void update(RWD& wd);
    bool die(RWD& wd);
    COORD getx();

    bool SaveInstance();
    bool LoadInstance();
    bool isIced();

    
};
// sf::Vertex Pea;


#endif