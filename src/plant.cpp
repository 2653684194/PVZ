#include "include/Plant.hpp"
#include "include/Bullet.hpp"

// #include <string>
// #include <iostream>
// #include <SFML/Graphics.hpp>

// const float xInc = 133;//increment

// const float xcast = 45;//define in head.hpp
// const float ycast = 55;//define in head.hpp

////////设置为-1以便debug知道传参是否失败
float SunFlower::SizeFactor = -1;
float PeaShooter::SizeFactor = -1;//静态成员初始化
float SnowPeaShooter::SizeFactor = -1;
float Repeater::SizeFactor = -1;
float WallNut::SizeFactor = -1;
float TallNut::SizeFactor = 1;


void PeaShooter::LoadTexture(){
    std::cout<<"=================PeaShooter LoadTexture"<<std::endl;
    FrameStartInd=::LoadTexture(ImgPath,frame);
    if (FrameStartInd==-1){
        std::cout<<"PeaShooter Load Texture Failed\n";
        return; 
    }
}
void SnowPeaShooter::LoadTexture() {
    std::cout<<"=================SnowPeaShooter LoadTexture"<<std::endl;
    FrameStartInd = ::LoadTexture(ImgPath, frame);
    if (FrameStartInd == -1){
        std::cerr << "SnowPea Load Texture Failed" << std::endl;
    }
}
void Repeater::LoadTexture() {
    std::cout<<"=================Repeater LoadTexture"<<std::endl;
    FrameStartInd = ::LoadTexture(ImgPath, frame);
    if (FrameStartInd == -1)
        std::cerr << "Repeater Load Texture Failed" << std::endl;

}
void SunFlower::LoadTexture(){
    std::cout<<"=================SunFlower LoadTexture"<<std::endl;
    FrameStartInd=::LoadTexture(ImgPath,frame);
    if (FrameStartInd==-1){
        std::cout<<"SunFlower Load Texture Failed\n";
        return ;
    }
    
}
void WallNut::LoadTexture() {
    std::cout<<"==================Wullnut LoadTexture"<<std::endl;
    NormalFrameStartInd= ::LoadTexture(NormalImgPath, NormalFrame);
    if (NormalFrameStartInd==-1){
        std::cout<<"WallNut Load Texture Failed\n";
        return;
    }
    Eaten1FrameStartInd = ::LoadTexture(Eaten1ImgPath, Eaten1Frame);
    if (Eaten1FrameStartInd==-1){
        std::cout<<"WallNut Load Texture Failed\n";
        return;
    }
    Eaten2FrameStartInd = ::LoadTexture(Eaten2ImgPath, Eaten2Frame);
    if (Eaten2FrameStartInd==-1){
        std::cout<<"WallNut Load Texture Failed\n";
        return;
    }
        
}
void TallNut::LoadTexture() {
    FrameStartInd = ::LoadTexture(ImgPath, frame);
    if (FrameStartInd == -1) {
        std::cout << "TallNut Load Texture Failed\n";
        return ;
    }
}


COORD Plant::getx()const{
    return x*CollisionDatas::xcast+CollisionDatas::PlantxInc+CollisionDatas::MAPxTranslation;
}

bool Plant::showauxiliary(short frame,short StartInd,float SizeFactor,float TimeLoop,float XInc,uint32_t brightness){
    if (!GamePaused&&GlobalTick%(int)(TickPerSec*TimeLoop/frame)==0)
        i=(i+1)%(frame);//每隔TimeLoop/frame刻数更新一次i
    
    if (StartInd+i<GlobalDynamicTextures.size()){
        sf::Sprite sp(GlobalDynamicTextures[StartInd+i]);//获取纹理

        sp.setPosition({getx(),y*CollisionDatas::ycast*1.0+CollisionDatas::PlantyInc});//设置坐标
        // std::cout<<SizeFactor<<std::endl;
        sp.setScale({SizeFactor,SizeFactor});
        sp.setColor(sf::Color(255*brightness,255*brightness,255*brightness));//设置颜色
        window.draw(sp);//绘制精灵
    }
    return true;
}


const HP SunFlowerHP = 100;//SunFlowerHP

//static val def 这些参数应该写在构造函数，避免内存占用
const TICK SunFlowerMaxActionInterval = TickPerSec*15;//SunFlowerTick 10 sec////////对于植物来说这是最大生成阳光时间
short SunFlower::SunAmount = 50;//SunFlowerSunAmount
const COST SunFlower::cost = 50;//SunFlowerCost
const TICK SunFlower::CoolDown = TickPerSec*7;
std::string SunFlower::ImgPath = "/img/Plant/SunFlower/";
short SunFlower::frame = 18;//这个保留在外部，这个其他地方用得到
short SunFlower::FrameStartInd = -1;

// const float SunFlowerSizeFactor = 0.6;

SunFlower::SunFlower(COORD xx,COORD yy):Plant(xx,yy){
    LastHp=hp=SunFlowerHP;
    //可能在没有调用frameStartInd时候被调用，导致访问不到FrameStardInd,增加保护
    if (FrameStartInd != -1 && SizeFactor == -1){
        sf::Texture* T=&GlobalDynamicTextures[FrameStartInd];
        SizeFactor = CollisionDatas::PlantSizeX *1.0/ (T->getSize().x * CollisionDatas::factor);//这里SizeY是未放大的//必须保证有ActualSizeFactor否则无法把sizefator传递给cardcard
    }    
    ActionInterval = SunFlowerMaxActionInterval; 
}
void SunFlower::show(sf::RenderWindow& wd){//必须是引用
    uint32_t b=1;
    if (hp-LastHp!=0){//detect attack
        b=150;
        if (!GamePaused){
            LastHp=hp;
        }
    }
    if (!showauxiliary(frame,FrameStartInd,SizeFactor,1,0,b))
        std::cout<<"SunFlower load img err!\n";
}
void SunFlower::action(){
    if (!GamePaused){
        TICK t=GlobalTick-TickGap>=0?GlobalTick-TickGap:GlobalTick-TickGap+TickPerSec*TimeLoop;
        srand(unsigned(time(nullptr))+TickGap);
        if (rand()%3==0)//每秒平均把ActionInterval-0.3s
            ActionInterval--;//集中在15.5s生成一次
        if (t>=ActionInterval){//不用偏移1//(分母，分子）
            std::cout<<"GlobalTick:"<<GlobalTick<<" t:"<<t<<std::endl;
            std::cout<<"SunFlower at ("<<x<<","<<y<<") produces Sun\n";
            // std::cout<<"x:"<<x*xcast*1.0+PlantxInc+MAPxTranslation<<" y:"<<y*ycast*1.0+PlantyInc<<std::endl;
            Sun* sun=new Sun(COORD(
                x*CollisionDatas::xcast*1.0+CollisionDatas::PlantxInc+CollisionDatas::PlantSizeX/(2*CollisionDatas::factor)),
                COORD(y*CollisionDatas::ycast*1.0+CollisionDatas::PlantyInc),
                COORD(y*CollisionDatas::ycast+CollisionDatas::PlantyInc+CollisionDatas::ycast+CollisionDatas::ycast*((rand()%10)/10)),
                Sun::State(Sun::State::fall|Sun::State::thrown));
            sunPool[y].insert(sun);
            TickGap=GlobalTick;
            ActionInterval=SunFlowerMaxActionInterval;
        }
    }
}
COST SunFlower::getCost()const{return cost;}
TICK SunFlower::getCoolDown()const{return CoolDown;}
short SunFlower::getFrameStartInd()const{return FrameStartInd;}

//////////////////////////////////////////////////////////////////////////////
bool SunFlower::SaveInstance(){
    std::ofstream outFile;

    //格式 username_savenum
    outFile.open(ParentPath+SavePath,std::ios::app);
    // 检查文件是否成功打开
    if (!outFile.is_open()) {
        std::cerr << "无法打开文件！" << std::endl;
        return 0;
    }
    outFile<<"SunFlower "<<x<<" "<<y<<std::endl\
    <<hp<<" "<<LastHp<<" "\
    <<short(i)<<" "<<TickGap<<" "\
    <<ActionInterval<<std::endl;
    return 1;
}
bool SunFlower::LoadInstance(){
    short ii;
    LoadReader>>hp>>LastHp>>ii>>TickGap>>ActionInterval;
    i=ii;
    std::cout<<"SunFlower "<<x<<" "<<y<<std::endl\
    <<hp<<" "<<LastHp<<" "\
    <<short(i)<<" "<<TickGap<<" "\
    <<ActionInterval<<std::endl;

    return 1;
}




const HP PeaShooterHP=100;//PeaShooterHP

TICK PeaShooter::ActionInterval=TickPerSec*1;//PeaShooterTick 1 sec
DG PeaShooter::dg = 10;//damage
const COST PeaShooter::cost = 100;//PeaShooterCost
const TICK PeaShooter::CoolDown = TickPerSec*7;
std::string PeaShooter::ImgPath="/img/Plant/PeaShooter/";// 像素大小71*71
short PeaShooter::frame=13;
short PeaShooter::FrameStartInd = -1;

// const float PeaShooterSizeFactor = 0.6;

PeaShooter::PeaShooter(COORD xx,COORD yy):Plant(xx,yy){
    LastHp=hp=PeaShooterHP;
//可能在没有调用frameStartInd时候被调用，导致访问不到FrameStardInd,增加保护
    if (FrameStartInd != -1 && SizeFactor == -1){
        sf::Texture* T=&GlobalDynamicTextures[FrameStartInd];
        SizeFactor = CollisionDatas::PlantSizeX *1.0/ (T->getSize().x * CollisionDatas::factor);//这里SizeY是未放大的//必须保证有ActualSizeFactor否则无法把sizefator传递给cardcard
    }
    PeaFlag=1;//1表示普通豌豆
}
void PeaShooter::show(sf::RenderWindow& wd){//必须是引用
    uint32_t b=1;
    if (hp-LastHp!=0){//detect attack
        b=150;
        if (!GamePaused){
            LastHp=hp;
        }
    }
    if (!showauxiliary(frame,FrameStartInd,SizeFactor,1,0,b))
        std::cout<<"PeaShooter load img err!\n";
}
void PeaShooter::action(){
    if (!GamePaused){
        if (!faceZombie(zombiePool[y],*this))
            return;
        TICK tick=GlobalTick-TickGap>0?GlobalTick-TickGap:GlobalTick-TickGap+TickPerSec*TimeLoop;
        if (tick%ActionInterval==0){
            std::cout<<"PeaShooter at ("<<x<<","<<y<<") shoots\n";
            using CD = CollisionDatas ;
            Bullet* bul=new Pea(\
                x*CD::xcast+CD::PlantxInc+CD::PlantSizeX/CD::factor-CD::PeaSizeX\
                ,y,100,dg,PeaFlag\
            );//这里不需要添加MapxTranslation,需要输入地图绝对坐标
            bulletPool[y].insert(bul);//插入到对应行的子弹池中
        }
    }
}
COST PeaShooter::getCost()const{return cost;}
TICK PeaShooter::getCoolDown()const{return CoolDown;}
short PeaShooter::getFrameStartInd()const{return FrameStartInd;}



bool PeaShooter::SaveInstance(){
    std::ofstream outFile;

    //格式 username_savenum
    outFile.open(ParentPath+SavePath,std::ios::app);
    // 检查文件是否成功打开
    if (!outFile.is_open()) {
        std::cerr << "无法打开文件！" << std::endl;
        return 0;
    }
    outFile<<"PeaShooter "<<x<<" "<<y<<std::endl\
    <<hp<<" "<<LastHp<<" "\
    <<short(i)<<" "<<TickGap<<std::endl;
    return 1;

}
bool PeaShooter::LoadInstance(){
    short ii;
    LoadReader>>hp>>LastHp>>ii>>TickGap;
    i=static_cast<uint8_t>(ii);
    std::cout<<"PeaShooter "<<x<<" "<<y<<std::endl\
    <<hp<<" "<<LastHp<<" "\
    <<short(i)<<" "<<TickGap<<std::endl;
    return 1;

}

//Snow Pea//
const COST SnowPeaShooter::cost = 175;
const TICK SnowPeaShooter::CoolDown = TickPerSec * 10;
std::string SnowPeaShooter::ImgPath = "/img/Plant/SnowPeaShooter/";
short SnowPeaShooter::frame = 15;
short SnowPeaShooter::FrameStartInd = -1;
DG SnowPeaShooter::dg = 5;
// const float SnowPeaShooterSizeFactor = 0.6;

SnowPeaShooter::SnowPeaShooter(COORD xx, COORD yy):PeaShooter(xx, yy) {
    LastHp = hp = 100;
    //可能在没有调用frameStartInd时候被调用，导致访问不到FrameStardInd,增加保护
    if (FrameStartInd != -1 && SizeFactor == -1){
        sf::Texture* T=&GlobalDynamicTextures[FrameStartInd];
        SizeFactor = CollisionDatas::PlantSizeX *1.0/ (T->getSize().x * CollisionDatas::factor);//这里SizeY是未放大的//必须保证有ActualSizeFactor否则无法把sizefator传递给cardcard
    }
    PeaFlag=2;
}

void SnowPeaShooter::show(sf::RenderWindow& wd) {
    uint32_t b = 1;
    if (hp < LastHp) {
        b = 150;
        if (!GamePaused) LastHp = hp;
    }
    if (!showauxiliary(frame,FrameStartInd, SizeFactor, 1.0, 0.0, b))
        std::cerr << "SnowPea show error" << std::endl;
}


//可以直接使用继承PeaShooter的函数
// void SnowPeaShooter::action() {
//     if (GamePaused) return;
//     if (!faceZombie(zombiePool[y], *this)) return;
//     TICK tick=GlobalTick-TickGap>0?GlobalTick-TickGap:GlobalTick-TickGap+TickPerSec*TimeLoop;
//     if (tick % ActionInterval == 0) {
//         Bullet* b = new Pea(x * xcast + PlantxInc + MAPxTranslation + 3, y, 100, dg, PeaFlag);
//         bulletPool[y].insert(b);
//     }
// }

COST SnowPeaShooter::getCost() const { return cost; }
TICK SnowPeaShooter::getCoolDown() const { return CoolDown; }
short SnowPeaShooter::getFrameStartInd() const { return FrameStartInd; }

bool SnowPeaShooter::SaveInstance() {
    std::ofstream out(ParentPath + SavePath, std::ios::app);
    if (!out.is_open()) return false;
    out << "SnowPea " << x << ' ' << y << ' '
        << hp << ' ' << LastHp << ' ' << int(i)
        << ' ' << TickGap << std::endl;
        return true;
}
//可以直接使用继承PeaShooter的函数
bool SnowPeaShooter::LoadInstance() {
    // short ii;
    // LoadReader >> hp >> LastHp >> ii >> TickGap;
    // i = ii;
    // return true;
    return PeaShooter::LoadInstance();//函数复用
}


//Repeater//
const COST Repeater::cost = 200;
const TICK Repeater::CoolDown = TickPerSec * 10;
std::string Repeater::ImgPath = "/img/Plant/Repeater/";
short Repeater::frame = 15;
short Repeater::FrameStartInd = -1;

const TICK Repeater::TwoPeasInterval = TickPerSec*0.2;

Repeater::Repeater(COORD xx, COORD yy):PeaShooter(xx, yy) {
    LastHp = hp = 100;
    PeaFlag = 1;
    //可能在没有调用frameStartInd时候被调用，导致访问不到FrameStardInd,增加保护
    if (FrameStartInd != -1 && SizeFactor == -1){
        sf::Texture* T=&GlobalDynamicTextures[FrameStartInd];
        SizeFactor = CollisionDatas::PlantSizeX *1.0/ (T->getSize().x * CollisionDatas::factor);//必须保证有ActualSizeFactor否则无法把sizefator传递给cardcard
    }
}



void Repeater::show(sf::RenderWindow& wd) {
    uint32_t b = 1;
    if (hp < LastHp) {
        b = 150;
        if (!GamePaused) LastHp = hp;
    }
    if (!showauxiliary(frame, FrameStartInd, SizeFactor, 1.0f, 0.0f, b))
        std::cerr << "Repeater show error" << std::endl;
}

void Repeater::action() {
    if (GamePaused) return;
    if (!faceZombie(zombiePool[y], *this)) return;
    TICK tick=GlobalTick-TickGap>0?GlobalTick-TickGap:GlobalTick-TickGap+TickPerSec*TimeLoop;
    TICK Peastick=GlobalTick-PeasTickGap>0?GlobalTick-PeasTickGap:GlobalTick-PeasTickGap+TickPerSec*TimeLoop + 1;
    if (tick % ActionInterval == 0) {
        PeasTickGap = GlobalTick;
        
        using CD = CollisionDatas ;
        Bullet* bul=new Pea(\
                x*CD::xcast+CD::PlantxInc+CD::PlantSizeX/CD::factor-CD::PeaSizeX\
                ,y,100,dg,PeaFlag\
            );//这里不需要添加MapxTranslation,需要输入地图绝对坐标
        bulletPool[y].insert(bul);
        
    }
    if (Peastick == TwoPeasInterval){
        using CD = CollisionDatas;
        Bullet* bul=new Pea(\
                x*CD::xcast+CD::PlantxInc+CD::PlantSizeX/CD::factor-CD::PeaSizeX\
                ,y,100,dg,PeaFlag\
            );//这里不需要添加MapxTranslation,需要输入地图绝对坐标
        bulletPool[y].insert(bul);
    }
}

COST Repeater::getCost() const { return cost; }
TICK Repeater::getCoolDown() const { return CoolDown; }
short Repeater::getFrameStartInd() const { return FrameStartInd; }

bool Repeater::SaveInstance() {
    std::ofstream out(ParentPath + SavePath, std::ios::app);
    if (!out.is_open()) return false;
    out << "Repeater " << x << " " << y << " "\
        << hp << " " << LastHp << " " << int(i)
        << " " << TickGap << " "<<PeasTickGap <<std::endl;
        return true;
}

bool Repeater::LoadInstance() {
    short ii;
    LoadReader >> hp >> LastHp >> ii >> TickGap >> PeasTickGap;
    i = ii;
    return true;
}


const HP WallNutHP = 500; // 5倍hp
// const float WallNutSizeFactor = 0.6;

std::string WallNut::NormalImgPath = "/img/Plant/WallNut/Normal/";
short WallNut::NormalFrame = 16;
short WallNut::NormalFrameStartInd = -1;
std::string WallNut::Eaten1ImgPath = "/img/Plant/WallNut/Eaten1/";
short WallNut::Eaten1Frame = 1;
short WallNut::Eaten1FrameStartInd = -1;
std::string WallNut::Eaten2ImgPath = "/img/Plant/WallNut/Eaten2/";
short WallNut::Eaten2Frame = 1;
short WallNut::Eaten2FrameStartInd = -1;

const COST WallNut::cost = 50;
const TICK WallNut::CoolDown = TickPerSec * 30; // 30 妙 cooldown

WallNut::WallNut(COORD xx, COORD yy) : Plant(xx, yy) {
    LastHp = hp = WallNutHP;
    //可能在没有调用frameStartInd时候被调用，导致访问不到FrameStardInd,增加保护
    if (NormalFrameStartInd != -1 && SizeFactor == -1){
        sf::Texture* T=&GlobalDynamicTextures[NormalFrameStartInd];
        SizeFactor = CollisionDatas::PlantSizeX *1.0/ (T->getSize().x * CollisionDatas::factor);//必须保证有ActualSizeFactor否则无法把sizefator传递给cardcard
    }
}

void WallNut::show(sf::RenderWindow& wd) {
    uint32_t brightness = 1;
    if (hp - LastHp != 0) { // 每一次被吃它会bright
        brightness = 150;
        LastHp = hp;
    }
    if (hp > WallNutHP * 0.75){
        if (!showauxiliary(NormalFrame, NormalFrameStartInd, SizeFactor, 1.5, 0, brightness)) {
            std::cout << "WallNut load img err!\n";
        }
    }else if (hp > WallNutHP * 0.33){
        i=0;
        if (!showauxiliary(Eaten1Frame, Eaten1FrameStartInd, SizeFactor, 1.5, 0, brightness)) {
            std::cout << "WallNut load img err!\n";
        }
    }else{
        i=0;
        if (!showauxiliary(Eaten2Frame, Eaten2FrameStartInd, SizeFactor, 1.5, 0, brightness)) {
            std::cout << "WallNut load img err!\n";
        }
    }
}

void WallNut::action(){
    ;
}

COST WallNut::getCost() const { return cost; }
TICK WallNut::getCoolDown() const { return CoolDown; }
short WallNut::getFrameStartInd() const { return NormalFrameStartInd; }

bool WallNut::SaveInstance(){
    std::ofstream out(ParentPath + SavePath, std::ios::app);
    if (!out.is_open()) return false;
    out << "WallNut " << x << " " << y << " "\
        << hp << " " << LastHp << " " << int(i) <<std::endl;
        return true;
}

bool WallNut::LoadInstance() {
    short ii;
    LoadReader >> hp >> LastHp >> ii;
    i = ii;
    std::cout << "WallNut " << x << " " << y << " "\
        << hp << " " << LastHp << " " << i <<std::endl;
    return true;
}


const HP TallNutHP = 1000; // 比普通WallNut两倍hp
// const float TallNutSizeFactor = 0.7;

std::string TallNut::ImgPath = "/img/Plant/TallNut/";
short TallNut::frame = 16;
short TallNut::FrameStartInd = -1;
const COST TallNut::cost = 125;
const TICK TallNut::CoolDown = TickPerSec * 50; // 50 妙 cooldown

TallNut::TallNut(COORD xx, COORD yy) : Plant(xx, yy) {
    LastHp = hp = TallNutHP;
    //可能在没有调用frameStartInd时候被调用，导致访问不到FrameStardInd,增加保护
    if (FrameStartInd != -1 && SizeFactor == -1){
        sf::Texture* T=&GlobalDynamicTextures[FrameStartInd];
        SizeFactor = CollisionDatas::PlantSizeX *1.0/ (T->getSize().x * CollisionDatas::factor);//必须保证有ActualSizeFactor否则无法把sizefator传递给cardcard
    }
}

void TallNut::show(sf::RenderWindow& wd) {
    uint32_t brightness = 1;
    if (hp - LastHp != 0) { // 每一次被吃它会bright
        brightness = 150;
        LastHp = hp;
    }
    if (!showauxiliary(frame,  FrameStartInd, SizeFactor, 1.5, 0, brightness)) {
        std::cout << "TallNut load img err!\n";
    }
}



