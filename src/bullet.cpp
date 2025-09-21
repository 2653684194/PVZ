#include "include/Bullet.hpp"

// const short yInc = 43;
// const short xInc = 133;

//无意义的、形式化的初始化
float Pea::SizeFactor = -1; 

std::string Pea::ImgPath = "/img/Bullet/Pea/";
// sf::Sprite* Pea::SParr[3]={nullptr,nullptr,nullptr};
sf::Texture Pea::TextureArr[3];



//绘制位置和实际碰撞箱子位置不一样
COORD Pea::getx(){return x/CollisionDatas::factor+CollisionDatas::PeaxInc+CollisionDatas::PeaSizeX+CollisionDatas::MAPxTranslation;}

Pea::Pea(COORD xx,COORD yy,SPD spd,DG dg,short flag):Bullet(xx,yy,spd,dg),PeaFlag(flag){
    std::cout<<"Check Pea x:"<<x/(CollisionDatas::factor*BulletOptimizeFactor)+CollisionDatas::MAPxTranslation<<std::endl;
}//默认参数在头文件

void Pea::LoadTexture(){//不可以使用精灵预处理，因为根本不能渲染
    std::cout<<"====================================Pea LoadTexture"<<std::endl;
    sf::Texture t;
    t.loadFromFile(ParentPath+ImgPath+"Pea.png");
    TextureArr[1]=t;
    sf::Texture t2;
    t2.loadFromFile(ParentPath+ImgPath+"IcePea.png");
    TextureArr[2]=t2;
    if (SizeFactor == -1)
        SizeFactor = CollisionDatas::PeaSizeY * 1.0 /t.getSize().y;/////一定要乘以1.0
    // std::cout<<CollisionDatas::PeaSizeY<<" =====================================   "<<t.getSize().y<<" ==================== "<<SizeFactor<<std::endl;
}

void Pea::move(RWD& wd){
    using CD = CollisionDatas;
    sf::Sprite sp(TextureArr[PeaFlag]);
    if (!GamePaused){
        if (GlobalTick%(int)(TickPerSec/(CD::factor*BulletOptimizeFactor))==0){
            x+=speed;
        }
    }
    sp.setScale({SizeFactor,SizeFactor});
    
    sp.setPosition({x/(CD::factor*BulletOptimizeFactor)+CD::MAPxTranslation,y*CD::ycast+CD::PlantyInc});
    wd.draw(sp);
}
void Pea::update(RWD& wd){
    move(wd);
}
// void Pea::LoadTexture(){
//     sf::Texture t;
//     t.loadFromFile(ParentPath+ImgPath+"Pea.png");
//     SParr[PeaFlag-'0']=new sf::Sprite(t);
//     (*SParr[PeaFlag-'0']).setScale({PeaSizeFactor,PeaSizeFactor});
// }
// void Pea::move(RWD& wd){
//     if (GlobalTick%(int)(TickPerSec/factor)==0){
//         x+=speed;
//     }
//     (*SParr[PeaFlag-'0']).setPosition({x/factor,y*ycast+yInc});
//     wd.draw(*SParr[PeaFlag-'0']);
// }
bool Pea::die(RWD& wd){
    return true;
}
bool Pea::isIced(){return PeaFlag == 2;}

bool Pea::SaveInstance(){
    std::ofstream outFile;

    //格式 username_savenum
    outFile.open(ParentPath+SavePath,std::ios::app);
    // 检查文件是否成功打开
    if (!outFile.is_open()) {
        std::cerr << "无法打开文件！" << std::endl;
        return 0;
    }
    outFile<<"Pea "<<x<<" "<<y<<std::endl\
    <<PeaFlag<<" "<<damage<<" "\
    <<speed<<std::endl;
    return 1;
}

bool Pea::LoadInstance(){
    LoadReader>>x>>y>>\
    PeaFlag>>damage\
    >>speed;
    std::cout<<std::endl<<"Pea "<<x<<" "<<y<<std::endl\
    <<PeaFlag<<" "<<damage<<" "\
    <<speed<<std::endl;
    return 1;

}
