#include "include/Instances.hpp"




std::string Sun::ImgPath = "/img/Sun/";
short Sun::Frame = 29;
float Sun::ImgInc = 39.5;
short Sun::FrameStartInd=-1;
short Sun::gravity=4;
COORD Sun::targetX = sunScore.Storex + 20;
COORD Sun::targetY = sunScore.Storey + 20;


void Sun::LoadTexture(){
    std::cout<<"=============Sun LoadTexture"<<std::endl;
    FrameStartInd=::LoadTexture(ImgPath,Frame,SizeFactor);
    if (FrameStartInd==-1)
    std::cout<<"Sun Load Texture Failed\n";
}
bool Sun::showauxiliary(short frame,short StartInd,float TimeLoop,uint32_t brightness){
    if (!GamePaused&&GlobalTick%(int)((TickPerSec*TimeLoop)/frame)==0)//一个TimeLoop播放一次动画
        i=(i+1)%(frame);
    if (StartInd+i<GlobalDynamicTextures.size()){
        using CD = CollisionDatas;
        sf::Sprite sp(GlobalDynamicTextures[StartInd+i]);
    //坐标转换CharToNum
        sp.setScale({SizeFactor,SizeFactor});
        sp.setPosition({x/CD::factor-ImgInc*SizeFactor + CD::MAPxTranslation,y/CollisionDatas::factor-ImgInc*SizeFactor});
        sp.setColor(sf::Color(255*brightness,255*brightness,255*brightness));
        window.draw(sp);
    }
    return true;
}
bool Sun::isclicked(){
    ///////////鼠标进入detect由sunpool维护
    if (!GamePaused){
        // std::cout<<"mouse:"<<myMouse.MousePosition.x<<" "<<myMouse.MousePosition.y<<"\tsun:"<<x/factor<<" "<<y/factor<<std::endl;
        short xgap=myMouse.MousePosition.x-(x/CollisionDatas::factor + CollisionDatas::MAPxTranslation);
        short ygap=myMouse.MousePosition.y-y/CollisionDatas::factor;
        static short range=13;
        if (xgap>=-range&&xgap<=range&&ygap>=-range&&ygap<=range){
            state=collected;
            myMouse.LClickProtection=0;

        }
    }
    return false;
}
short Sun::ExitTime = 10;
short range2 = 8;
void Sun::update(){
    
    if (state&State::dead||SizeFactor<=0.02){//((x/factor)-targetX<range2&&(x/factor)-targetX>-range2&&(y/factor)-targetY<range2&&(y/factor)-targetY>-range2)
        sunScore.Sun+=25;
        state=State::dead;
        // std::cout<<"die";
        return;
    }
    ////////由tick控制基本不需要判断paused///////////////////////////////////////////////////////////////////////////////////////////
    TICK tick=GlobalTick-TickGap>=0?GlobalTick-TickGap:GlobalTick-TickGap+TickPerSec*TimeLoop;//必须大于等于0
    // std::cout<<"tick"<<tick<<" ExitTime*TickPecSec:"<<ExitTime*TickPerSec<<"\n";
    float tickgap = (TickPerSec*1.0/CollisionDatas::factor);
    uint32_t b=1;
    if (state!=collected&&tick>=ExitTime*TickPerSec){
        state=dead;
        return;
    }else if (state!=collected&&tick>ExitTime*TickPerSec*0.8){
        if ((ExitTime*TickPerSec-tick)/2&&tick%((ExitTime*TickPerSec-tick)/2)==0)
            b=150;
    }
    if (!GamePaused){
        if (state!=collected&&(GroundY*CollisionDatas::factor-y<=ySpeed*tickgap||GroundY-y/CollisionDatas::factor<=range2)){
            ySpeed*=-0.9;
            if (int(ySpeed/range2)==0)//到达边界附近加速度为零双重检测
                state=normal;
        }
        if (x/CollisionDatas::factor-0<range2||WINDOW_WIDTH*CollisionDatas::factor-x<=thrownspeed*tickgap)
            thrownspeed*=-0.9;

        ////////////这里用到==判断，要是恰好暂停在这里会无限更新
        if (state&&tick%(short)tickgap==0){
            if ((short)state&(short)fall){    
                ySpeed+=gravity*tickgap;//因为x被放大处理，所以Speed相当于缩小处理
            }
            if ((short)state&(short)thrown){//包含状态检测
                x+=thrownspeed*tickgap;        
            }
            if ((short)state&(short)collected){
                // std::cout<<"iscollected";
                x-=(x/CollisionDatas::factor+CollisionDatas::MAPxTranslation-targetX);//因为x被factor放大处理，所以这个位移相当于除以factor
                y-=(y/CollisionDatas::factor-targetY);
                if (SizeFactor>0)
                    SizeFactor-=0.007;
            
            }else 
                y+=ySpeed*tickgap;//向上初速度在创建时定义
        }
    }
    if (showauxiliary(Frame,FrameStartInd,2.0,b))
            ;
}

bool Sun::SaveInstance(){
    std::ofstream outFile;

    //格式 username_savenum
    outFile.open(ParentPath+SavePath,std::ios::app);
    // 检查文件是否成功打开
    if (!outFile.is_open()) {
        std::cerr << "无法打开文件！" << std::endl;
        return 0;
    }
    outFile<<"Sun "<<x<<" "<<y<<std::endl\
    <<short(i)<<" "<<TickGap<<" "\
    <<state<<" "<<GroundY<<" "<<std::fixed<<std::setprecision(10)<<ySpeed<<" "<<thrownspeed<<" "<<SizeFactor<<std::endl;

    return 1;

}
bool Sun::LoadInstance(){
    
    short st,ii;
    LoadReader>>x>>y>>ii>>TickGap>>st>>GroundY>>ySpeed>>thrownspeed>>SizeFactor;
    state = State(st);
    i=ii;

    std::cout<<"Sun "<<x<<" "<<y<<std::endl\
    <<short(i)<<" "<<TickGap<<" "\
    <<state<<" "<<GroundY<<" "<<std::fixed<<std::setprecision(10)<<ySpeed<<" "<<thrownspeed<<" "<<SizeFactor<<std::endl;
    return true;

  
}



std::string SunScore::ImgPath="/img/Num.png";
std::string SunScore::PlantStoreImgPath="/img/PlantStore.png";
float SunScore::SizeFactor = 0.02;
short SunScore::Storex=40;
short SunScore::Storey=0;
std::vector<sf::Texture> SunScore::ScoreNums;

bool SunScore::LoadTexture(){
    if (!PlantStoreTexture.loadFromFile(ParentPath+PlantStoreImgPath)){
        std::cout<<"Load err\n";
        return false;
    }

    sf::Image fullimg;
    if (!fullimg.loadFromFile(ParentPath+ImgPath)){
        std::cout<<"Load Num err\n";
        return false;
    }//
    //{125,425-450,800-820,1150-1165,1489}
    int x[5]={110,450,805,1150,1489};
    int sizex[5]={340,350,345,339,340};
    for (short i=0;i<10;i++){
        sf::Texture t;
        sf::IntRect IR;

        IR.position.x=x[i%5];
        IR.position.y=20+(i/5)*480;
        IR.size.x=sizex[i%5];
        IR.size.y=480;
        
        if(!t.loadFromImage(fullimg,false,IR)){
            std::cout<<"Load Num err\n";
            return false;
        }
        // std::cout<<"push_back\n";
        ScoreNums.push_back(t);//第一个是1
    }
    return true;
}
void SunScore::update(){
    if (Sun>=10000)
        Sun=9999;
    // float brightness=1.2;

    short i=SizeFactor*345*4+Storex;
    
    sf::Sprite SB(PlantStoreTexture);
    // SB.setColor(sf::Color(255*brightness,255*brightness,255*brightness));
    SB.setScale({0.5,0.5});
    SB.setPosition({Storex,Storey});
    window.draw(SB);
    
    short s=Sun;
    if (!s){
        sf::Sprite sp(ScoreNums[9]);//+9而不是-1防止负数
        if (insufficient){
            sp.setColor(sf::Color(255,0,0));
        }
        else{
            sp.setColor(sf::Color(1,1,1));
        }
        // sp.setColor(sf::Color(255,100*brightness,100*brightness));
        sp.setScale({SizeFactor,SizeFactor});
        sp.setPosition({i,Storey+31});
        window.draw(sp);
    }
    while (s){
        sf::Sprite sp(ScoreNums[(s+9)%10]);//+9而不是-1防止负数
        // sp.setColor(sf::Color(255,100*brightness,100*brightness));
        if (insufficient){
            sp.setColor(sf::Color(255,0,0));
        }
        else{
            sp.setColor(sf::Color(1,1,1));
        }
        sp.setScale({SizeFactor,SizeFactor});
        sp.setPosition({i,Storey+31});
        i-=SizeFactor*344;
        s/=10;
        window.draw(sp);
        
    }
    if (insufficient&&!GamePaused)
        insufficient--;

}


bool SunScore::SaveInstance(){
    std::ofstream outFile;

    //格式 username_savenum
    outFile.open(ParentPath+SavePath,std::ios::app);
    // 检查文件是否成功打开
    if (!outFile.is_open()) {
        std::cerr << "can't open file" << std::endl;
        return 0;
    }
    outFile<<"SunScore "<<Sun<<" "<<short(insufficient)<<std::endl;

    return 1;

}
bool SunScore::LoadInstance(){
    std::string str;
    short ii;
    LoadReader>>str;
    if (str=="SunScore"){
        LoadReader>>Sun>>ii;
        insufficient=ii;
        return 1;
    }
    return 0;
}


