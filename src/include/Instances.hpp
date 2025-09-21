#ifndef INSTANCES_HPP
#define INSTANCES_HPP

#include "Head.hpp"


class MyMouse;
template<class instance>
class Map;
class Plant;

template<class instances>
class ZombiePool;
template<class instances>
class BulletPool;

template <class instances>
class BasicPool;

class SunPool;
class CardPool;

class Zombie;
class Plant;
class Bullet;

class Sun;
class SunScore;
class CardBase;

template <typename T1,typename T2>
void Prework(BulletPool<T1>& bulletPool, ZombiePool<T2>& zombiePool);
template <typename T1,typename T2>
void BPInteractWithZP(BulletPool<T1>& bulletPool, ZombiePool<T2>& zombiePool);

template <class T>
bool faceZombie(ZombiePool<T>&,Plant&);

class MyMouse{
public:
    bool MouseIsEnter;
    sf::Vector2i MousePosition;
    _Float16 MouseInGridX,MouseInGridY;
    bool LClickProtection;
    bool LButtonPressed;
    bool RClickProtection;
    bool RButtonPressed;

    CardBase* occupied;

    void update();
    
    void changeState(CardBase* cb);
    inline bool isoccupied()const{return occupied;} 
};
inline MyMouse myMouse;

template<class instance>
class ZombiePool{
    uint8_t line;
    struct node{
        node* pre;
        instance* item;
        node* next;
        node(instance* i):pre(nullptr),item(i),next(nullptr){}
    };
    node* Head;
    // node** cur
    std::vector<node*> Fronts;
    void swap(node* a,node* b);
public:
    ZombiePool(){Head=nullptr;}
    ~ZombiePool();
    void init(short n){line=n;}//初始化行数
    void insert(instance* item);
    void update(RWD& wd);
    void clear();
    void save();
    void load();
    template <typename T1,typename T2>
    friend void Prework(BulletPool<T1> &BP,ZombiePool<T2> &ZP);//交互函数的预处理函数
    template <typename T1,typename T2>
    friend void BPInteractWithZP(BulletPool<T1>& bulletPool, ZombiePool<T2>& zombiePool);//子弹池和僵尸池交互函数

    template <class T>
    friend bool faceZombie(ZombiePool<T>&,Plant&);//判断植物是否面对僵尸
    
};
template<class instance>
class BulletPool{
    uint8_t line;
    struct node{
        node* pre;
        instance* item;
        node* next;
        node(instance* i):pre(nullptr),item(i),next(nullptr){}
    };
    node* Head;
    // node** cur
    
    void swap(node* a,node* b);
    std::vector<node*> Fronts;
public:
    
    BulletPool(){Head=nullptr;}
    ~BulletPool();
    void init(short n){line=n;}
    void insert(instance* item);
    void update(RWD& wd);
    void clear();
    void save();
    void load();
    template <typename T1,typename T2>
    friend void Prework(BulletPool<T1> &BP,ZombiePool<T2> &ZP);//交互函数的预处理函数

    HP* GetZombieHP(Zombie* zb);
    void SlowDownActivated(Zombie* zb);//冰豌豆减速激活
    DG GetBulletDG(Bullet* bl);

    template <typename T1,typename T2>
    friend void BPInteractWithZP(BulletPool<T1>& bulletPool, ZombiePool<T2>& zombiePool);//子弹池和僵尸池交互函数
};

template <class instance>
class BasicPool{
public:
    struct node{
        instance* item;
        node* next;
        node(instance* i):item(i),next(nullptr){}
    };
// protected:
    node* Head;
    node* rear;
public:
    BasicPool(){
        Head=nullptr;
        rear=nullptr;
    }
    ~BasicPool(){
        node* t;
        while(Head){
            t=Head;
            Head=Head->next;
            delete t->item;
            delete t;
        }
    }
    void insert(instance* item){
        if (!Head){
            Head=new node(item);
            rear=Head;
            return;
        }
        rear->next=new node(item);
        rear=rear->next;
    }
    void clear(){
        node* t;
        while(Head){
            t=Head;
            Head=Head->next;
            delete t->item;
            delete t;
        }
    }
};

class SunPool:public BasicPool<Sun>{
public:
    uint8_t line;

    SunPool():BasicPool(){}
    ~SunPool(){BasicPool<Sun>::~BasicPool();}
    void init(short i){line=i;}

    void update();

    void save();
    void load();
    
};
class CardPool:public BasicPool<CardBase>{
private:
    CardBase* cards[13];
public:
    CardPool():BasicPool(){}
    ~CardPool(){BasicPool<CardBase>::~BasicPool();}

    void update();
    void setCardPos(COORD x,COORD y,CardBase* cb);//设置卡片位置

    void save();
    void load();
};

const int MAP_LENGTH = 9;
const int DAY_MAP_WIDTH = 5;

template<class instance>
class Map{
    static uint8_t PlantedNum;
    instance* map[MAP_LENGTH][DAY_MAP_WIDTH];
    // instance* front[DAY_MAP_WIDTH];//好像不需要
public:
    Map();
    bool insert(instance* item,short x,short y);
    bool erase(short x,short y);
    instance* at(short x,short y);
    void update(sf::RenderWindow& wd,uint16_t line);

    void clear();
    void save();
    void load();
    static uint8_t NumOfPlants();
};


class Zombie{
protected:
    //value defined in derived class
    HP hp;
    HP LastHp;
    COORD x,y;//x 是实际的pixel y是僵尸当前所在路径
    COORD DesinatedY,yTransalation;

    const static TICK SlowDownTime;//冰豌豆减速时间
    TICK SlowDownTimeRemain;//to modify speed when hitten by ice pea
    const static _Float16 SlowDownFactor;//冰豌豆减速因子

    //value defined in this class
    TICK TickGap;
    uint8_t i;//植物的帧参数可用？
    bool ismove;
    bool dieflag;
    bool ImgChangeFlag;
    bool isslowdown;
    bool switchOnce;

    static std::string HeadFallImgPath;
    static short HeadFallFrameStartIndex;
    static short HeadFallFrame;

public:
    Zombie(COORD xx=500,COORD yy=0):x(xx*CollisionDatas::factor),y(yy){//对x放大处理实现较高精度,
        i=0;//可能在子类中某些函数需要重新初始化
        TickGap=GlobalTick;
        DesinatedY = yy;
        yTransalation = 0;
        ismove=1;
        dieflag=1;
        ImgChangeFlag=0;
        isslowdown = 0;
        SlowDownTimeRemain = 0;
        switchOnce=0;
    };
    virtual void walk(RWD& wd)=0;
    virtual void eat(RWD& wd,Plant*)=0;
    virtual void update(RWD& wd)=0;//需要为bullet单独设计Pool
    virtual void LoadTexture()=0;
    virtual bool die(RWD& wd)=0;
    virtual ~Zombie()=default;

    bool SwitchLane();//切换僵尸所在路径

    bool showauxiliary(short frame,short Ind,float SizeFactor,float timeloop,float xInc=0,uint32_t brightness=1);
    void UpdateSlowDown();//更新减速状态
    HP* GetPlantHP(Plant* pl);

    template <class instance>
    friend HP* BulletPool<instance>::GetZombieHP(Zombie* zb);
    template <class instance>
    friend void BulletPool<instance>::SlowDownActivated(Zombie* zb);


    virtual COORD getx()=0;
    virtual COORD getGridx()=0;

    COORD gety()const{return y;}
    COORD getDestY()const{return DesinatedY;}
    HP GetZombieHP()const{return hp;}

    virtual bool SaveInstance()=0;
    virtual bool LoadInstance()=0;

    void GameLose();//游戏失败,僵尸到达房子
};
inline ZombiePool<Zombie> zombiePool[5];//全局对象池声明在这里

class Plant{
protected:  
    //value defined in derived class
    HP hp;//植物的生命值
    HP LastHp;//植物的上一次生命值
    COORD x,y;//坐标
      
    //value defined in this class
    TICK TickGap;//植物行为时间戳
    uint8_t i;//帧参数
public:
    Plant(COORD xx,COORD yy):x(xx),y(yy){
        i=0;
        TickGap=GlobalTick;
    }
    virtual void show(sf::RenderWindow&)=0;//显示植物
    virtual void action()=0;//植物行为
    
    virtual ~Plant()=default;
    friend HP* Zombie::GetPlantHP(Plant* pl);//获取植物的生命值指针

    COORD getx()const;//获取植物的x坐标
    virtual COST getCost()const=0;//获取植物的消耗阳光
    virtual TICK getCoolDown()const=0;//获取植物的冷却时间
    virtual short getFrameStartInd()const=0;//获取植物的帧在GlobalDynamicTextures起始索引

    
    template <class T>
    friend bool faceZombie(ZombiePool<T>&,Plant&);//判断植物前方是否有僵尸

    bool showauxiliary(short frame,short StartInd,float,float TimeLoop,float xInc=0,uint32_t brightness=1);//辅助函数,用于显示植物的动画

    virtual bool SaveInstance()=0;//保存植物实例到文件
    virtual bool LoadInstance()=0;//从文件加载植物实例
};
inline Map<Plant> GlobalMap;//全局对象池声明在这里


const short BulletOptimizeFactor = 1;
class Bullet{
protected:
    COORD x,y;
    SPD speed;
    DG damage;
public:
    Bullet(COORD xx,COORD yy,SPD spd,DG dg):x(xx*CollisionDatas::factor*BulletOptimizeFactor),y(yy),speed(spd),damage(dg){}//x:实际坐标的放大倍,y:整型坐标
    // virtual ~Bullet()=0;
    virtual void LoadTexture()=0;
    virtual void move(RWD& wd)=0;
    virtual void update(RWD& wd)=0;
    virtual bool die(RWD& wd)=0;
    // bool isCollide();
    //子弹先消失,僵尸后播放死亡动画
    // virtual void die()=0;
    // virtual bool isCollide(Zombie* zom)=0;//后面需要把接口改成Pool?//或者,Pool删除只做剔除不做内存delete//Pool做成全局变量 Pool1 2 3 4 5 表示每一行
    template<class T>
    friend DG BulletPool<T>::GetBulletDG(Bullet* bl);

    virtual COORD getx()=0;
    COORD gety()const{return y;}
    DG get()const{return damage;}

    virtual bool SaveInstance()=0;
    virtual bool LoadInstance()=0;
    virtual bool isIced()=0;
    

};
inline BulletPool<Bullet> bulletPool[5];//全局对象池声明在这里

class Sun{
public:
    enum State{//类内美剧类型必须在public域
        normal=0,
        fall=1,
        thrown=2,
        collected=4,
        dead=8
    };
private:
    State state;

    COORD x,y; 
    static COORD targetX,targetY;
    COORD GroundY;
    SPD ySpeed;
    SPD thrownspeed;
    
    uint8_t i;
    TICK TickGap;

    static short gravity;//重力加速度
    static short ExitTime;//阳光消失时间

    static std::string ImgPath;
    static short Frame;
    static short FrameStartInd;
    static float ImgInc;//用于找到中心点坐标

    float SizeFactor;

public:
    Sun(COORD xx=0,COORD yy=0,COORD groundy=WINDOW_HEIGHT-5,State st = normal):
    x(xx*CollisionDatas::factor),y(yy*CollisionDatas::factor),GroundY(groundy),i(0),TickGap(GlobalTick),state(st){
        SizeFactor = 0.5;
        ySpeed=0;
        if (st&thrown){
            ySpeed=-40-30*((rand()%11)*1.0/10);
            thrownspeed=18*((rand()%2)?1:-1)+12*((rand()%21-10)/10);//包含负数
        }
    }
    void LoadTexture();
    bool showauxiliary(short frame,short StartInd,float TimeLoop,uint32_t brightness = 1);

    bool isclicked();//判断阳光是否被点击
    void update();

    inline float getx(){return x*1.0/CollisionDatas::factor;}//图片坐标默认左上角，所以要+Increment
    inline float gety(){return y*1.0/CollisionDatas::factor;}
    inline bool isdie(){return state&State::dead;}//判断阳光是否死亡（消失）

    bool SaveInstance();
    bool LoadInstance();
};
inline SunPool sunPool[6];//全局对象池声明在这里


class SunScore{
public:
    uint8_t insufficient;//是否阳光不足
    short Sun;//阳光数量
    static std::vector<sf::Texture> ScoreNums;//存储数字纹理
    sf::Texture PlantStoreTexture;//植物商店纹理
    SunScore():Sun(0),insufficient(0){}

    static short Storex,Storey;
    static std::string PlantStoreImgPath;
    static std::string ImgPath;
    static float SizeFactor;
    void update();
    bool LoadTexture();//加载纹理

    bool SaveInstance();
    bool LoadInstance();
};
inline SunScore sunScore;

class CardBase{
public:
    enum State{
        coolingdown=0,
        normal=1,
        draged=2
        
    }state;//由于不可抗拒的关系,必须声明为公有方便不同实例化之间的访问
protected:
    COORD x,y;//坐标,单位为像素
    COORD originX,originY;//原始位置,用于拖动时恢复位置

    TICK TickGap;//时间戳,用于计算冷却时间
    TICK CoolDown;//冷却时间

    sf::Vector2u cardSize,InstanceSize;

    static std::string ImgPath;
    sf::Texture CardTexture;
    sf::Texture InstanceTexture;
    float SizeFactor;
    float InstanceSizeFactor;
    float CoolDownImgFactor;

    std::string InstanceName;//实例名称,用于保存和加载

    short Cost;
public:
    CardBase(COORD xx,COORD yy):x(xx*CollisionDatas::factor),y(yy*CollisionDatas::factor),originX(xx),originY(yy),state(normal){
        if (!CardTexture.loadFromFile(ParentPath+ImgPath)){
            std::cout<<"Load Card img err\n";
        }
        SizeFactor=0.26;
    }
    virtual void Init(short StartInd,short cost,TICK CoolDown,float instancesizefactor,std::string)=0;
    virtual void show(){std::cout<<"I can't be abstract class\n";}//作为接口必须显示定义但定义无用东西
    virtual void update(){std::cout<<"I can't be abstract class\n";}
    virtual bool SaveInstance()=0;
    virtual bool LoadInstance()=0;
    // void show();
    // void update();

    friend void MyMouse::changeState(CardBase* cb);
    friend void CardPool::setCardPos(COORD x,COORD y,CardBase*);
};

template <class T>
class Card:public CardBase{
public:
    template <typename U>
    friend class Card;

    Card(COORD xx=200,COORD yy=3):CardBase(xx,yy){}
    void Init(short StartInd,short cost,TICK cooldown,float instancesizefactor,std::string str);//初始化卡片
    void show();
    void update();

    bool SaveInstance();
    bool LoadInstance();
};
inline CardPool cardPool;//全局对象池声明在这里

#endif