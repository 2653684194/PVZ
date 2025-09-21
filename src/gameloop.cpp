#include "include/GameLoop.hpp"

#include <chrono>
#include <thread>

//不能定义为常量，这个需要在程序运行时赋值
// std::string ParentPath;

sf::Texture Background;
sf::Texture Surface;
sf::Texture Lose;
sf::Texture Loader[12];

short iii=0;
void LoaderAnimaton(){ 
    for (short i=0;i<2;i++){
        window.clear(sf::Color::Black);
        sf::Sprite tsp(Loader[iii]);
        tsp.setPosition({20,20});
        tsp.setScale({1.2,1.2});
        window.draw(tsp);
        window.display();
        iii=(iii+1)%12;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void Init(){//////////////////负责静态初始化
    std::string LoaderImgPath = "/img/Loader/";
    
    for (short i=1;i<=12;i++){
        if (!Loader[i-1].loadFromFile(ParentPath+LoaderImgPath+std::to_string(i)+".png")){
            std::cerr<<"Load Loader Texture err\n";
        }
    }
    short iii=0;
    LoaderAnimaton();

    //打标记，记录行标签
    for (short i=0;i<5;i++){
        zombiePool[i].init(i);
        bulletPool[i].init(i);
        sunPool[i].init(i);
        bulletPool[i].init(i);
    }
    sunPool[5].init(5);//////代表下落sun
    LoaderAnimaton();
    
    // 加载纹理
    if (!Background.loadFromFile(ParentPath+"/img/Background/Day.jpg")) {
        std::cerr << "Load Background Img err!" << std::endl;
    }if (!Surface.loadFromFile(ParentPath+"/img/UI/Menu/Surface.png")) {
        std::cerr << "Load Background Img err!" << std::endl;
    }if (!Lose.loadFromFile(ParentPath+"/img/Lose.png")){
        std::cerr << "Load Lose Img err!" << std::endl;
    }
    LoaderAnimaton();
    
    dialog_exit.Init();/////////////////////////////必须确保dialog在其他地方初始化

    LoaderAnimaton();

    dialog_save.Init(0,"yes","no","save or not?");

    LoaderAnimaton();

    dialog_restart.Init(0,"load","restart","load latest saved?");

    LoaderAnimaton();

    pauseButton.Init();

    LoaderAnimaton();

    screenAdventure.Init();

    LoaderAnimaton();

    sunScore.LoadTexture();

    LoaderAnimaton();

}

void Load(bool reset){//负责加载动态材质到GlobalDynamicTextures，如不重启，读取存档
    Zombie* tZombie=new RegularZombie;
    Zombie* coneheadZombie = new ConeheadZombie;
    Bullet* bul=new Pea;
    Sun* sun=new Sun;
////加载僵尸、子弹、阳光材质
    tZombie->LoadTexture();
    delete tZombie;
    coneheadZombie->LoadTexture();
    delete coneheadZombie;
    (bul)->LoadTexture();
    delete bul;
    sun->LoadTexture();
    delete sun;

    LoaderAnimaton();
      
    //判断重启标签，加载所有植物及其对应的卡牌材质，并判断是否加载所有实例，
    if (!reset&&std::filesystem::exists(ParentPath+SavePath) && std::filesystem::file_size(ParentPath+SavePath)!=0){
        LoadReader.close();
        LoadReader.open(ParentPath+SavePath);//////////////相当于打开＋定位到开头
        LoadReader.seekg(0);
        std::string str;
        LoadReader>>str;
        std::cout<<"================================================="<<str<<std::endl;
        if (str=="Game_Saved"){
            std::cout<<"===============================Load Game================================\n";
            LoadReader>>str;
            if (str=="GlobalTick"){
                LoadReader>>GlobalTick;
                std::cout<<"GloabalTick: "<<GlobalTick<<std::endl;
            }
            for(short i=0;i<DAY_MAP_WIDTH;i++){
                zombiePool[i].load();

                LoaderAnimaton();
            }
            GlobalMap.load();
            for (short i=0;i<DAY_MAP_WIDTH;i++){
                std::cout<<"================================BulletPool in gameloop\n";
                bulletPool[i].load();

                LoaderAnimaton();
            }
            for (short i=0;i<6;i++){
                std::cout<<"================================SunPool in gameloop\n";
                sunPool[i].load();

                LoaderAnimaton();
            }
            cardPool.load();

            LoaderAnimaton();

            sunScore.LoadInstance();

            LoaderAnimaton();
        }
    }else{ 
        sunScore.Sun = 100;
        PeaShooter::LoadTexture();
        SunFlower::LoadTexture();
        SnowPeaShooter::LoadTexture();
        Repeater::LoadTexture();
        WallNut::LoadTexture();

        LoaderAnimaton();

        Plant* plantPtr=new PeaShooter;
        // plantPtr->LoadTexture();
        Card<PeaShooter>* card =new Card<PeaShooter>;
        std::cout<<" ============== "<<((PeaShooter*)plantPtr)->SizeFactor<<std::endl;
        card->Init(plantPtr->getFrameStartInd(),plantPtr->getCost(),plantPtr->getCoolDown(),((PeaShooter*)plantPtr)->SizeFactor,"PeaShooter");
        cardPool.insert(card);
        delete plantPtr;

        LoaderAnimaton();

        plantPtr=new SunFlower;
        // plantPtr->LoadTexture();
        Card<SunFlower>* card2 =new Card<SunFlower>;
        card2->Init(plantPtr->getFrameStartInd(),plantPtr->getCost(),plantPtr->getCoolDown(),((SunFlower*)plantPtr)->SizeFactor,"SunFlower");
        cardPool.insert(card2);
        delete plantPtr;

        LoaderAnimaton();

        plantPtr = new SnowPeaShooter;
        // plantPtr->LoadTexture();
        Card<SnowPeaShooter>* card3 = new Card<SnowPeaShooter>;
        card3->Init(plantPtr->getFrameStartInd(), plantPtr->getCost(), plantPtr->getCoolDown(), ((SnowPeaShooter*)plantPtr)->SizeFactor, "SnowPea");
        cardPool.insert(card3);
        delete plantPtr;

        LoaderAnimaton();

        plantPtr = new Repeater;
        // plantPtr->LoadTexture();
        Card<Repeater>* card4 = new Card<Repeater>;
        card4->Init(plantPtr->getFrameStartInd(), plantPtr->getCost(), plantPtr->getCoolDown(), ((Repeater*)plantPtr)->SizeFactor, "Repeater");
        cardPool.insert(card4);
        delete plantPtr;

        LoaderAnimaton();

        plantPtr = new WallNut;
        // plantPtr->LoadTexture();
        Card<WallNut>* card5 = new Card<WallNut>;
        card5->Init(plantPtr->getFrameStartInd(), plantPtr->getCost(), plantPtr->getCoolDown(), ((WallNut*)plantPtr)->SizeFactor, "WallNut");
        cardPool.insert(card5);
        delete plantPtr;

        LoaderAnimaton();

    }

}

void Save(){//写存档
    //创建顺序必须和加载顺序一致
    std::ofstream outFile;
    outFile.open(ParentPath+SavePath,std::ios::out);
    outFile<<"Game_Saved"<<std::endl;
    outFile<<"GlobalTick "<<GlobalTick<<std::endl;/////////////////必须放在这里否则会因为缓冲区问题输出顺序调转
    for(short i=0;i<DAY_MAP_WIDTH;i++){
        zombiePool[i].save();
    }
    GlobalMap.save();
    for (short i=0;i<DAY_MAP_WIDTH;i++){
        bulletPool[i].save();
    }
    for (short i=0;i<6;i++){
        sunPool[i].save();
    }
    cardPool.save();
    
    sunScore.SaveInstance();
}

void Clear(){//清空所有实例，重置标签及数据
    GameLosed = 0;
    CollisionDatas::MAPxTranslation = -70;//用于失败动画
    //调用各个对象池封装的clear函数，清空对应实例
    for (short i=0;i<DAY_MAP_WIDTH;i++){
        zombiePool[i].clear();
        bulletPool[i].clear();
        sunPool[i].clear();
    }
    sunPool[5].clear();
    GlobalMap.clear();
    cardPool.clear();
    GlobalDynamicTextures.clear();
}

const static TICK SunFallMinInterval = 8 * TickPerSec;
static TICK TickRemain = SunFallMinInterval;
static TICK TickGap = GlobalTick;

void GameLoop(){
    if (!GamePaused){
        srand((unsigned)time(nullptr));
        // ///////////////////////////////////////////////////////随机生成
        // if (GlobalTick%int(TickPerSec*10)==0){//10 sec
        //     COORD x=rand()%MAP_LENGTH;
        //     COORD y=rand()%DAY_MAP_WIDTH;
        //     Plant* temp=new PeaShooter(x,y);
        //     if (GlobalMap.insert(temp,x,y))//因为设置了不能重复plant所以重复new的植物会内存溢出
        //         std::cout<<"successfully planted\n";
        //     else
        //         delete temp;
            
        //     x=rand()%MAP_LENGTH;
        //     y=rand()%DAY_MAP_WIDTH;
        //     temp=new SunFlower(x,y);
        //     if (GlobalMap.insert(temp,x,y))
        //         std::cout<<"successfully planted\n";
        //     else
        //         delete temp;
        // }
        ////////////随机生成阳光
        if (TickRemain==0){
            TickRemain = SunFallMinInterval;
            Sun* sun= new Sun(rand()%WINDOW_WIDTH,0,315,Sun::fall);
            sunPool[5].insert(sun);
        }
        if (rand()%3!=0){
            TickRemain--;
        }

        //////////////////////随机生成僵尸，难度随植物数量上升
        short Num=GlobalMap.NumOfPlants();
        short ttt;
        if (Num<=10)
            ttt=20*TickPerSec - Num * TickPerSec * 0.45;
        else if (Num<15)
            ttt=15*TickPerSec - Num * TickPerSec * 0.33;
        else if (Num<20)
            ttt=12.5*TickPerSec - Num * TickPerSec * 0.276;
        else if (Num<25)
            ttt=10*TickPerSec - Num * TickPerSec * 0.2;
        else if (Num<30)
            ttt=7.5*TickPerSec - Num * TickPerSec * 0.15;  
        if (ttt <= 0)
            ttt=1;
        TICK tick=GlobalTick-TickGap>=0?GlobalTick-TickGap:GlobalTick-TickGap+TickPerSec*TimeLoop;
        
        if (tick>ttt){
            short yy=rand()%DAY_MAP_WIDTH;
            Zombie* temp;
            if (rand()%2==0)//随机选择僵尸生成
                temp=new ConeheadZombie(WINDOW_WIDTH-5,yy);
            else 
                temp=new RegularZombie(WINDOW_WIDTH-5,yy);

            zombiePool[yy].insert(temp);
            if (GlobalMap.NumOfPlants()>20){//如果植物数量超过20，增加ConeheadZombie的生成
                short yy=rand()%DAY_MAP_WIDTH;
                Zombie* temp=new ConeheadZombie(WINDOW_WIDTH-5,yy);
                zombiePool[yy].insert(temp);
            }
            TickGap=GlobalTick;//重置时间间隔
        }
    }

    ////////////////////////////////更新所有对象池
    sunScore.update();
    cardPool.update();
    for (short i=0;i<DAY_MAP_WIDTH;i++){
        
        bulletPool[i].update(window);
        GlobalMap.update(window,i);
        
        zombiePool[i].update(window);
        sunPool[i].update();
    }
    sunPool[5].update();
/////////////////////////////////////////////////////UI交互应该独立出来
    // dialog.update();
    // pauseButton.update();

    // window.display();//移交渲染函数

    // TickUpdate();//转交时间计算函数
}
