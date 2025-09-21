#include "include/Zombie.hpp"
#include "include/UI.hpp"

// const short ycast=55;//define in head
// const short yInc=4;//increment

std::string Zombie::HeadFallImgPath = "/img/Zombie/HeadFall/";
short Zombie::HeadFallFrame = 12;
short Zombie::HeadFallFrameStartIndex = -1;

const TICK Zombie::SlowDownTime = TickPerSec * 10;
const _Float16 Zombie::SlowDownFactor = 0.5;

//静态成员无意义的初始化
float RegularZombie::SizeFactor = -1;


void RegularZombie::LoadTexture(){
    std::cout<<"=============RegularZombie LoadTexture"<<std::endl;
    DieFrameStartIndex=::LoadTexture(DieImgPath,DieFrame);//使用“::”获取外部的LoadTexture函数而不是本身
    EatFrameStartIndex=::LoadTexture(EatImgPath,EatFrame);
    WalkFrameStartIndex=::LoadTexture(WalkImgPath,WalkFrame);
    HeadFallFrameStartIndex=::LoadTexture(HeadFallImgPath,HeadFallFrame);
    if (DieFrameStartIndex==-1||EatFrameStartIndex==-1||WalkFrameStartIndex==-1){
        std::cout<<"Zombie Load Texture Failed\n";
        return;
    }
    //为了正确初始化sizefactor 如果不采用静态loadtexture则需要在这里初始化
    sf::Texture* T=&GlobalDynamicTextures[WalkFrameStartIndex];
    SizeFactor = CollisionDatas::ZombieSizeY *1.0 / (T->getSize().y);//这里SizeY是未放大的
    
}
void ConeheadZombie::LoadTexture() {
    std::cout<<"=============ConeheadZombie LoadTexture"<<std::endl;
    WalkFrameStartIndex = ::LoadTexture(WalkImgPath, WalkFrame);
    EatFrameStartIndex = ::LoadTexture(EatImgPath, EatFrame);
    DieFrameStartIndex = ::LoadTexture(DieImgPath, DieFrame);

    if (WalkFrameStartIndex == -1 || EatFrameStartIndex == -1 ) {
        std::cerr << "ConeheadZombie LoadTexture failed!" << std::endl;
        return;
    }

    // 初始化 SizeFactor
    sf::Texture* T = &GlobalDynamicTextures[WalkFrameStartIndex];
    SizeFactor = CollisionDatas::ZombieSizeY * 1.0f / (T->getSize().y);
}

//由于僵尸走路每一步时动态的，需要调整
//没有inline竟然编译不了？
bool Zombie::showauxiliary(short frame,short StartInd,float SizeFactor,float TimeLoop,float XInc,uint32_t brightness){
    _Float16  SDfactor = isslowdown?SlowDownFactor:1;//减速因子
    if (!GamePaused&&GlobalTick%(int)((TickPerSec*TimeLoop) / (frame * SDfactor))==0)//一个TimeLoop播放一次动画
        i=(i+1)%(frame);
    if (StartInd+i<GlobalDynamicTextures.size()){
        sf::Sprite sp(GlobalDynamicTextures[StartInd+i]);
    //坐标转换CharToNum
        // std::cout<<"ActualSizeFatcor"<<ActualSizeFactor<<std::endl;
        sp.setScale({SizeFactor,SizeFactor});
        sp.setPosition({x/CollisionDatas::factor+CollisionDatas::MAPxTranslation,y*CollisionDatas::ycast+CollisionDatas::ZombieyInc+yTransalation});
        if (isslowdown){
            sp.setColor(sf::Color(128*brightness,128*brightness,255*brightness));
        }else{
            sp.setColor(sf::Color(255*brightness,255*brightness,255*brightness));
        }
        window.draw(sp);//绘制精灵
        
    }
    return true;
}
void Zombie::UpdateSlowDown(){
    if (isslowdown&&!GamePaused){//采用二重判断
        if (SlowDownTimeRemain==0){//如果是第一次减速
            SlowDownTimeRemain=SlowDownTime+1;//+1 是为了修正
        }
        SlowDownTimeRemain--;
        if (SlowDownTimeRemain==0){//减速时间结束
            isslowdown = 0;
        }
    }
}
bool Zombie::SwitchLane(){
    if (GamePaused)return false;
    COORD yGap = (DesinatedY - y) * CollisionDatas::ycast;//计算y轴的间隔
    if (yGap == 0){
        switchOnce = 0;
        return false;
    }
    static _Float16 OptFactor = 0.04;//optimize factor 动画优化因子
    TICK tick=GlobalTick-TickGap>0?GlobalTick-TickGap:GlobalTick-TickGap+TickPerSec*TimeLoop;

    TICK gap=(short) TickPerSec * 1.4 * OptFactor;// 1.4 Sec
    if (gap==0)gap=1;
    // std::cout<<yGap-yTransalation<<" "<<yGap * OptFactor * 1.5<<" "<<short(yGap-yTransalation)/(yGap * OptFactor * 1.5)<<std::endl;
    if (tick %  gap== 0 && (short)((yGap-yTransalation)/(yGap * OptFactor * 1.5)) != 0){
        yTransalation += yGap * OptFactor;
    }
    if ((short)((yGap-yTransalation)/(yGap * OptFactor * 1.5)) == 0){
        y=DesinatedY;//影响到showauxiliary
        yTransalation = 0;
    }
    if (!switchOnce&&yTransalation*2.0/yGap>=1){
        switchOnce=1;
        return true;
    }
    return false;
}

void Zombie::GameLose(){
    static TICK TimeRemain;
    float t=3.0;
    if (!GamePaused){
        TimeRemain = TickPerSec*t;
        GamePaused = 1;
    }
    if (GamePaused){

        float fac = (TickPerSec*t-TimeRemain)/(TickPerSec * t)+0.3;//计算缩放因子
        if (fac > 0.8)fac=0.8;

        if (CollisionDatas::MAPxTranslation<0){//重置地图平移
            CollisionDatas::MAPxTranslation += 70.0 / TickPerSec*1.5;
        }
        TimeRemain--;

    }

    //动画处理
    if (GamePaused && TimeRemain <= 0.5*TickPerSec*t && GameLosed == 0){ 
        GameLosed = 1;
    }
    if (GamePaused&&TimeRemain==0){
        GameActivated=0;
        UIstate.ScreenAdventureState=UIState::State::display;
    }
}

std::string RegularZombie::DieImgPath = "/img/Zombie/RegularZombie/Die/";//这种格式竟然也可以
short RegularZombie::DieFrame = 10;
short RegularZombie::DieFrameStartIndex = -1;
std::string RegularZombie::EatImgPath = "/img/Zombie/RegularZombie/Eat/";
short RegularZombie::EatFrame = 21;
short RegularZombie::EatFrameStartIndex = -1;
std::string RegularZombie::WalkImgPath = "/img/Zombie/RegularZombie/Walk/";
short RegularZombie::WalkFrame = 18;
short RegularZombie::WalkFrameStartIndex = -1;

SPD RegularZombie::speed = 10;// pixel per second?  <20
TICK RegularZombie::ActionInterval = 1 * TickPerSec;//1 second
DG RegularZombie::dg = 20;

const COORD RegularZombie::ImgInc = 65;

const HP RegularZombieHP = 100;


RegularZombie::RegularZombie(COORD x,COORD y):Zombie(x,y){
    LastHp=hp=RegularZombieHP;
    if (WalkFrameStartIndex != -1 && SizeFactor == -1){
        
        // sf::Texture* T=&GlobalDynamicTextures[WalkFrameStartIndex];
        // SizeFactor = CollisionDatas::ZombieSizeY *1.0 / (T->getSize().y);//这里SizeY是未放大的
        // std::cout<<"debug ====="<<CollisionDatas::ZombieSizeY<<"=========="<<T->getSize().y<<"==================="<<SizeFactor<<std::endl;
    }
}


COORD RegularZombie::getx(){return x/CollisionDatas::factor+ImgInc*SizeFactor+CollisionDatas::MAPxTranslation;}//和直接的x坐标不同，这个是用来获取碰撞箱坐标的
COORD RegularZombie::getGridx(){return (getx()-CollisionDatas::PlantxInc-CollisionDatas::MAPxTranslation)/CollisionDatas::xcast;}


//由于僵尸走路每一步时动态的，需要调整
void RegularZombie::walk(RWD& wd){
    uint32_t b=1;
    _Float16 SDfactor = isslowdown?SlowDownFactor:1;
    if (hp-LastHp!=0){//detect attack
        b=150;
        if (!GamePaused){
            LastHp=hp;
        }
    }
    TICK tick=GlobalTick-TickGap>0?GlobalTick-TickGap:GlobalTick-TickGap+TickPerSec*TimeLoop;
    if (!showauxiliary(WalkFrame,WalkFrameStartIndex,SizeFactor,1.8,0,b))
        std::cout<<"RegularZombie load WalkImg err\n";
    
        //SDfactor 既可以设置在speed 也可以设置在时间判定
    if (!GamePaused&&ismove&&!isstop()&&tick%(int)(TickPerSec/ CollisionDatas::factor)==0){// 1/factor second
        x-=speed * SDfactor;//因为x在作用时使用了移位运算处理
        // std::cout<<"RegularZombie at ("<<x/factor<<","<<y*ycast+yInc<<") move\n";
    }
    
}
void RegularZombie::eat(RWD& wd,Plant* pl){
    uint32_t b=1;
    _Float16  SDfactor = isslowdown?SlowDownFactor:1;
    if (hp-LastHp!=0){//detect attack
        b=150;
        if (!GamePaused){
            LastHp=hp;
        }
    }
    short optimizefactor=4;//数值越大，使得攻击伤害更加连贯,同时可以让植物受伤闪烁更快,但是会给缓存增加负担
    showauxiliary(EatFrame,EatFrameStartIndex,SizeFactor,1.5,-10,b);
    TICK tick=GlobalTick-TickGap>0?GlobalTick-TickGap:GlobalTick-TickGap+TickPerSec*TimeLoop;
    if (!GamePaused&&tick%(int)(ActionInterval/optimizefactor)==0){
        // std::cout<<"RegularZombie at ("<<x/factor<<","<<y*ycast+yInc<<") attack\n";
        HP* plhp=GetPlantHP(pl);
        *plhp-=dg* SDfactor /optimizefactor;//无法访问hp在植物处处理删除逻辑
        if (*plhp<=0)//这个删除在在Map做好还是这里？
            GlobalMap.erase((getx()-CollisionDatas::PlantxInc-CollisionDatas::MAPxTranslation)*1.0/CollisionDatas::xcast,y);
        
    }
}
void RegularZombie::update(RWD& wd){//这个接口应该在Pool实现
    // if (hp<=0)
    //     return;
    Plant* pl=nullptr;//必须声明nullptr
    // std::cout<<(x/factor-xLEdgeOfFirstGrid)<<std::endl;
    float xx=getGridx();
    if (xx<0){
        DesinatedY = 2;
    }
    SwitchLane();//切换僵尸所在路径
    UpdateSlowDown();//更新减速状态
    // if (int(xx)+0.1<xx&&xx<int(xx)+0.7)
    pl=GlobalMap.at(xx,y);
    if (getx()<CollisionDatas::PlantxInc * 0.5+CollisionDatas::MAPxTranslation){//如果僵尸走到地图左边
        GameLose();//游戏失败
    }
    // if (pl)
    //     std::cout<<pl->getx()<<" "<<getx()<<std::endl;
    if (pl&&pl->getx()<getx()&&getx()<pl->getx()+CollisionDatas::PlantSizeX*0.6/CollisionDatas::factor){
        i=i%EatFrame;//保证i在EatFrame范围内
        eat(wd,pl);
    }else{
        i=i%WalkFrame;//保证i在WalkFrame范围内
        walk(wd);
    }
}

bool RegularZombie::die(RWD& wd){
    if (dieflag){
        std::cout<<"no more slowdown"<<std::endl;
        isslowdown = 0;
        i=0;
        dieflag=0;
    }
    // TICK tick=GlobalTick-TickGap>0?GlobalTick-TickGap:GlobalTick-TickGap+TickPerSec*TimeLoop;
    float dietimeloop = 1.7;
    if (i<DieFrame-1&&!showauxiliary(DieFrame,DieFrameStartIndex,SizeFactor,dietimeloop))
        std::cout<<"RegularZombie load DieImg err\n";
    //因为showauxilary自带i自增 //i<DieFrame-1保证只在die需要加载时维护i
    if (!GamePaused&&i<DieFrame-1&&GlobalTick%(int)((TickPerSec*dietimeloop)/DieFrame)==0)//一个TimeLoop播放一次动画
        i--;
    if (!showauxiliary(HeadFallFrame,HeadFallFrameStartIndex,SizeFactor,dietimeloop,20))//动画不能被完整播放
        std::cout<<"RegularZombie load HeadFallImg err\n";
    // std::cout<<"i:"<<int(i)<<std::endl;
    if (i+1==HeadFallFrame){//如果i+1==HeadFallFrame说明已经播放完毕
        std::cout<<"Zombie at ("<<x/CollisionDatas::factor<<","<<y<<") die\n";
        return true;
    }
    return false;
}

bool RegularZombie::SaveInstance(){
    std::ofstream outFile;

    //格式 username_savenum
    outFile.open(ParentPath+SavePath,std::ios::app);
    // 检查文件是否成功打开
    if (!outFile.is_open()) {
        std::cerr << "无法打开文件！" << std::endl;
        return 0;
    }
    outFile<<"RegularZombie "<<x<<" "<<y<<" "<<DesinatedY<<" "<<yTransalation<<std::endl\
    <<hp<<" "<<LastHp<<" "\
    <<short(i)<<" "<<TickGap<<" "\
    <<ismove<<" "<<dieflag<<" "<<ImgChangeFlag<<" "
    <<SlowDownTimeRemain<<" "<<isslowdown<<" "<<switchOnce<<std::endl;
    return 1;
}
bool RegularZombie::LoadInstance(){
    short ii;
    LoadReader>>x>>y>>DesinatedY>>yTransalation>>hp>>LastHp>>ii>>TickGap>>ismove>>dieflag>>ImgChangeFlag>>SlowDownTimeRemain>>isslowdown>>switchOnce;
    i=ii;
    std::cout<<"RegularZombie "<<x<<" "<<y<<" "<<DesinatedY<<" "<<yTransalation<<std::endl\
    <<hp<<" "<<LastHp<<" "\
    <<short(i)<<" "<<TickGap<<" "\
    <<ismove<<" "<<dieflag<<" "<<ImgChangeFlag<<" "
    <<SlowDownTimeRemain<<" "<<isslowdown<<" "<<switchOnce<<std::endl;
    return 1;

}

// ======== ConeheadZombie =========

const COORD ConeheadZombie::ImgInc = 65;
float ConeheadZombie::SizeFactor = -1;
TICK ConeheadZombie::ActionInterval = 1 * TickPerSec;  
DG ConeheadZombie::dg = 20;
SPD ConeheadZombie::speed = 10;
const HP ConeheadZombieHP = 200;

std::string ConeheadZombie::DieImgPath = "/img/Zombie/RegularZombie/Die/";
short ConeheadZombie::DieFrameStartIndex = -1;
short ConeheadZombie::DieFrame = 10;

std::string ConeheadZombie::EatImgPath = "/img/Zombie/ConeHeadZombie/Eat/";
short ConeheadZombie::EatFrameStartIndex = -1;
short ConeheadZombie::EatFrame = 11;

std::string ConeheadZombie::WalkImgPath = "/img/Zombie/ConeHeadZombie/Walk/";
short ConeheadZombie::WalkFrameStartIndex = -1;
short ConeheadZombie::WalkFrame = 21;

ConeheadZombie::ConeheadZombie(COORD xx, COORD yy) : RegularZombie(xx, yy) {
    LastHp = hp = ConeheadZombieHP;  // 比正常Zombie更多hp
}

COORD ConeheadZombie::getx(){return x/CollisionDatas::factor+ImgInc*SizeFactor+CollisionDatas::MAPxTranslation;}//和直接的x坐标不同，这个是用来获取碰撞箱坐标的
COORD ConeheadZombie::getGridx(){return (getx()-CollisionDatas::PlantxInc-CollisionDatas::MAPxTranslation)/CollisionDatas::xcast;}

void ConeheadZombie::walk(RWD& wd) {
    uint32_t b = 1;
    _Float16 SDfactor = isslowdown ? SlowDownFactor : 1;
    if (hp - LastHp != 0) {
        b = 150;
        if (!GamePaused) {
            LastHp = hp;
        }
    }
    TICK tick = GlobalTick - TickGap > 0 ? GlobalTick - TickGap : GlobalTick - TickGap + TickPerSec * TimeLoop;
    if (!showauxiliary(WalkFrame, WalkFrameStartIndex, SizeFactor, 1.8, 0, b))
        std::cout << "ConeheadZombie load WalkImg err\n";

    if (!GamePaused && ismove && !isstop() && tick % (int)(TickPerSec / CollisionDatas::factor) == 0) {
        x -= speed * SDfactor;
    }
}


void ConeheadZombie::eat(RWD& wd, Plant* pl) {
    uint32_t b = 1;
    _Float16 SDfactor = isslowdown ? SlowDownFactor : 1;
    if (hp - LastHp != 0) {
        b = 150;
        if (!GamePaused) {
            LastHp = hp;
        }
    }
    short optimizefactor = 4;
    showauxiliary(EatFrame, EatFrameStartIndex, SizeFactor, 1.5, -10, b);
    TICK tick = GlobalTick - TickGap > 0 ? GlobalTick - TickGap : GlobalTick - TickGap + TickPerSec * TimeLoop;
    if (!GamePaused && tick % (int)(ActionInterval / optimizefactor) == 0) {
        HP* plhp = GetPlantHP(pl);
        *plhp -= dg * SDfactor / optimizefactor;
        if (*plhp <= 0)
            GlobalMap.erase((getx() - CollisionDatas::PlantxInc - CollisionDatas::MAPxTranslation) * 1.0 / CollisionDatas::xcast, y);
    }
}

void ConeheadZombie::update(RWD& wd) {
    Plant* pl=nullptr;//必须声明nullptr

    float xx=getGridx();
    if (xx<0){
        DesinatedY = 2;
    }
    SwitchLane();
    UpdateSlowDown();
    // if (int(xx)+0.1<xx&&xx<int(xx)+0.7)
    pl=GlobalMap.at(xx,y);
    if (getx()<CollisionDatas::PlantxInc * 0.5+CollisionDatas::MAPxTranslation){
        GameLose();
    }
    // if (pl)
    //     std::cout<<pl->getx()<<" "<<getx()<<std::endl;
    if (pl&&pl->getx()<getx()&&getx()<pl->getx()+CollisionDatas::PlantSizeX*0.6/CollisionDatas::factor){
        i=i%EatFrame;
        eat(wd,pl);
    }else{
        i=i%WalkFrame;
        walk(wd);
    }
}


bool ConeheadZombie::die(RWD& wd) {
    if (dieflag) {
        isslowdown = 0;
        i = 0;
        dieflag = 0;
    }
    float dietimeloop = 1.7;
    if (i < DieFrame - 1 && !showauxiliary(DieFrame, DieFrameStartIndex, SizeFactor, dietimeloop))
        std::cout << "ConeheadZombie load DieImg err\n";
    if (!GamePaused && i < DieFrame - 1 && GlobalTick % (int)((TickPerSec * dietimeloop) / DieFrame) == 0)
        i--;
    if (!showauxiliary(HeadFallFrame, HeadFallFrameStartIndex, SizeFactor, dietimeloop, 20))
        std::cout << "ConeheadZombie load HeadFallImg err\n";
    if (i + 1 == HeadFallFrame) {
        std::cout << "ConeheadZombie at (" << x / CollisionDatas::factor << "," << y << ") die\n";
        return true;
    }
    return false;
}

bool ConeheadZombie::SaveInstance(){
    std::ofstream outFile;

    //格式 username_savenum
    outFile.open(ParentPath+SavePath,std::ios::app);
    // 检查文件是否成功打开
    if (!outFile.is_open()) {
        std::cerr << "无法打开文件！" << std::endl;
        return 0;
    }
    outFile<<"ConeheadZombie "<<x<<" "<<y<<" "<<DesinatedY<<" "<<yTransalation<<std::endl\
    <<hp<<" "<<LastHp<<" "\
    <<short(i)<<" "<<TickGap<<" "\
    <<ismove<<" "<<dieflag<<" "<<ImgChangeFlag<<" "
    <<SlowDownTimeRemain<<" "<<isslowdown<<" "<<switchOnce<<std::endl;
    return 1;
}
bool ConeheadZombie::LoadInstance(){
    short ii;
    LoadReader>>x>>y>>DesinatedY>>yTransalation>>hp>>LastHp>>ii>>TickGap>>ismove>>dieflag>>ImgChangeFlag>>SlowDownTimeRemain>>isslowdown>>switchOnce;
    i=ii;
    std::cout<<"ConeheadZombie "<<x<<" "<<y<<" "<<DesinatedY<<" "<<yTransalation<<std::endl\
    <<hp<<" "<<LastHp<<" "\
    <<short(i)<<" "<<TickGap<<" "\
    <<ismove<<" "<<dieflag<<" "<<ImgChangeFlag<<" "
    <<SlowDownTimeRemain<<" "<<isslowdown<<" "<<switchOnce<<std::endl;
    return 1;

}

