// #include "include/Instances.hpp"
#include "include/Plant.hpp"


std::string CardBase::ImgPath="/img/Card.png";
// sf::Texture CardBase::CardTexture;
// float CardBase::SizeFactor=0.5;

// In card.cpp
template class Card<PeaShooter>;  // For each concrete plant type
template class Card<SunFlower>;
template class Card<SnowPeaShooter>;
template class Card<Repeater>;
template class Card<WallNut>;

void MyMouse::changeState(CardBase* cb){
    cb->state=(CardBase::State)(cb->state&(~CardBase::State::draged));
}


template<class instance>
void Card<instance>::Init(short StartInd,short cost,TICK cooldown,float instancesizefactor,std::string str){
    InstanceName = str;

    InstanceTexture=GlobalDynamicTextures[StartInd];//获取实例纹理
    Cost=cost;//初始化实例价格
    CoolDown=cooldown;//初始化实例冷却时间
    InstanceSizeFactor=instancesizefactor;//初始化实例纹理缩放比例
    
    cardSize=CardTexture.getSize();
    InstanceSize=InstanceTexture.getSize();

    float tempSizeFactor=cardSize.x*1.0/InstanceSize.x*0.8;
    sf::Sprite Card(CardTexture);
    sf::Sprite Instance(InstanceTexture);

    Card.setPosition({0,0});
    Instance.setScale({tempSizeFactor,tempSizeFactor});
    Instance.setPosition({cardSize.x*(1-0.8)/2,cardSize.x*0.2});
    sf::RenderTexture RT({cardSize.x,cardSize.y});//创建渲染纹理

    RT.draw(Card);
    RT.draw(Instance);
    // SunScore font;
    short tempx=cardSize.x;
    while(cost){//绘制价格
        std::cout<<cost%10<<std::endl;
        sf::Sprite sp(sunScore.ScoreNums[(cost+9)%10]);
        sp.setScale({0.045,0.045});
        sp.setColor({1,1,1});
        sp.setPosition({tempx*0.5,cardSize.y*0.8});
        tempx-=27;
        cost/=10;
        RT.draw(sp);
    }
    RT.display();//渲染纹理
    CardTexture=RT.getTexture();//获取渲染纹理
}
template<class instance>
void Card<instance>::show(){
    sf::Sprite Card(CardTexture);
    if (state&normal){
        if (state&draged){
            sf::Color col=Card.getColor();
            TICK t=GlobalTick%TickPerSec-TickPerSec/2;
            if (t<0)
                t*=-1;
            col.a=256*(1-t*1.0/TickPerSec);//设置透明度
            Card.setColor(col);

            float xx=myMouse.MouseInGridX,yy=myMouse.MouseInGridY;
            //预览植物动画
            if (xx>=0 && xx<MAP_LENGTH && yy>=0 && yy<DAY_MAP_WIDTH && !GlobalMap.at(xx,yy)){//如果鼠标在地图范围内且该位置没有植物
                sf::Sprite Instance(InstanceTexture);
                Instance.setScale({InstanceSizeFactor,InstanceSizeFactor});
                sf::Color col=Instance.getColor();
                col.a = 255*0.7;
                Instance.setColor(col);
                Instance.setPosition({int(xx)*CollisionDatas::xcast+CollisionDatas::PlantxInc+CollisionDatas::MAPxTranslation,int(yy)*CollisionDatas::ycast+CollisionDatas::PlantyInc});
                window.draw(Instance);
            }
        }
    }
    Card.setScale({SizeFactor,SizeFactor});
    Card.setPosition({originX,originY});
    window.draw(Card);
    

    if(state==coolingdown){//冷却动画
        sf::RectangleShape RTShape;
        sf::Color col=sf::Color::Black;
        col.a=256*0.5;
        RTShape.setFillColor(col);
        RTShape.setSize(sf::Vector2f({cardSize.x*SizeFactor,cardSize.y*SizeFactor*CoolDownImgFactor}));
        RTShape.setPosition(sf::Vector2f({originX,originY+cardSize.y*SizeFactor*(1-CoolDownImgFactor)}));
        window.draw(RTShape);
        
    }
}
template<class instance>
void Card<instance>::update(){
    if (!GamePaused){
        if (myMouse.LClickProtection&&myMouse.LButtonPressed){//鼠标左键按下意味着以及进入了窗口///&&myMouse.LClickProtection
            sf::Vector2i MPos=myMouse.MousePosition;
            float xx=myMouse.MouseInGridX,yy=myMouse.MouseInGridY;
            
            if (state&normal){//如果卡片处于正常状态
                if (MPos.x>=originX&&MPos.x<=originX+cardSize.x*SizeFactor&&MPos.y>=originY&&MPos.y<=originY+cardSize.y*SizeFactor){ 
                    myMouse.LClickProtection=0;
                    if (sunScore.Sun < Cost){
                        sunScore.insufficient=5;//5 frame show time
                    }else if (myMouse.occupied!=this){//如果鼠标没有占用当前卡片
                        if (myMouse.occupied)//防止是unllptr
                            myMouse.changeState(myMouse.occupied);
                        myMouse.occupied = this;
                        state=(State)(state|draged);
                        // std::cout<<"MousePos:("<<MPos.x<<","<<MPos.y<<")\n";
                        // std::cout<<"cardSize:("<<cardSize.x<<","<<cardSize.y<<")\n";
                    }
                }else if ((state&draged)&& xx>=0 && xx<MAP_LENGTH && yy>=0 && yy<DAY_MAP_WIDTH && !GlobalMap.at(xx,yy)){
                    Plant* pl;
                    pl=new instance(xx,yy);
                    GlobalMap.insert(pl,xx,yy);
                    myMouse.occupied=nullptr;//清除鼠标占用的卡片
                    sunScore.Sun -= Cost;

                    state=(State)(state&(~draged));

                    state=coolingdown;
                    TickGap=GlobalTick;//设置冷却时间戳

                    myMouse.occupied = nullptr;//清除鼠标占用的卡片
                    
                }else if(state&draged&& !(xx>=0 && xx<MAP_LENGTH && yy>=0 && yy<DAY_MAP_WIDTH)){
                    state=(State)(state&(~draged));//这个两个必须绑定，否则会因为本Card已占用但是draged被更新了
                    myMouse.occupied = nullptr;//清除鼠标占用的卡片
                }
            }
            //
        }
        if(!(state&normal)){//鼠标不参与自动更新
            TICK tick=GlobalTick-TickGap>0?GlobalTick-TickGap:GlobalTick-TickGap+TickPerSec*TimeLoop;
            // std::cout<<"GlobalTick: "<<GlobalTick<<"tick in card"<<tick<<"\n";
            CoolDownImgFactor = (CoolDown-tick)*1.0/CoolDown;
            // std::cout<<"check factor: "<<CoolDownImgFactor<<std::endl;
            if (GlobalTick-TickGap==CoolDown||GlobalTick-TickGap+TickPerSec*TimeLoop==CoolDown){///////////未来肯出错>=更好

                state=normal;
            }
        }
        if (myMouse.RButtonPressed){
            state=(State)(state&(~draged));
            myMouse.occupied = nullptr;
        }
        // if (state==clicked&&myMouse.LButtonPressed&&myMouse.LClickProtection){

        // }
        
        // show();
    }
}
    
template <class instance>
bool Card<instance>::SaveInstance(){
    std::ofstream outFile;

    //格式 username_savenum
    outFile.open(ParentPath+SavePath,std::ios::app);
    // 检查文件是否成功打开
    if (!outFile.is_open()) {
        std::cerr << "无法打开文件！" << std::endl;
        return 0;
    }
    outFile<<InstanceName<<"_Card "<<originX<<" "<<originY<<" "<<x<<" "<<y<<std::endl\
    <<Card::state<<" "<<Card::CoolDownImgFactor<<" "<<Card::TickGap<<std::endl;
    
    return 1;

}

template <class instance>
bool Card<instance>::LoadInstance(){/////结合Init使用
    // 检查文件是否成功打开
    if (!LoadReader){
        std::cerr << "Can't open LoadReader" << std::endl;
        return 0;
    }
    std::cout<<"load card"<<std::endl;
    short st;
    LoadReader>>originX>>originY>>x>>y\
    >>st>>CoolDownImgFactor>>TickGap;
    state=State(st);

    std::cout<<InstanceName<<"_Card "<<originX<<" "<<originY<<" "<<x<<" "<<y<<std::endl\
    <<Card::state<<" "<<Card::CoolDownImgFactor<<" "<<TickGap<<std::endl;
    
    return 1;


}