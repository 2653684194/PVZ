#include "include/Plant.hpp"




//////////为了解决模板类的非模板子类不能在次头文件定义的尴尬集中定义

void CardPool::setCardPos(COORD xx,COORD yy,CardBase* cb){
    cb->originX=xx,cb->originY=yy;
}

void CardPool::update(){
    node* temp=Head;
    uint16_t x=sunScore.Storex+40,y=3;
    while (temp){
        CardBase* cb=temp->item;
        setCardPos(x,y,cb);
        cb->update();
        cb->show();
        x+=27;

        temp=temp->next;
    }

}

void SunPool::update(){

    node* temp=Head,*pre=nullptr;
    while(temp){
        Sun* item=temp->item;
        item->update();
        node* tempnext=temp->next;

        // std::cout<<"SunPoolIs updated\n";
        if (myMouse.LClickProtection&&myMouse.LButtonPressed){
            // std::cout<<"isclicked\n";
            if (item->isclicked())//如果被点击了
                myMouse.LClickProtection=0;
        }//删除实例
        if (item->isdie()){//如果被点击了或者超出范围
            if (pre){
                (pre)->next=tempnext;
                if (!tempnext)
                    rear=pre;//只需考虑尾部时处理
            }else{
                Head=tempnext;
            }
            delete item;
            delete temp;
        }else{   
            pre=temp;
        }
        temp=tempnext;
    }
}

void CardPool::save(){
    std::ofstream outFile;
    outFile.open(ParentPath+SavePath,std::ios::app);
    if (!outFile.is_open()){
        std::cout<<"open File:xxx In CardPool err\n";
    }

    outFile<<"CardPool_Content"<<std::endl;

    node* t=Head;
    while(t){
        t->item->SaveInstance();
        t=t->next;
    }

    outFile<<"ContentEnd"<<std::endl; 
}
void CardPool::load(){
    Plant* plantPtr;
    std::string str;
    // if (!LoadReader.is_open())
    //     std::cout<<"errrrrrrr"<<std::endl;
    std::cout<<"11111111111111111111111111111111111111111111111111111\n";
    while(LoadReader>>str){
        std::cout<<"============================================read success"<<std::endl;
        std::cout<<str<<std::endl;
        if (str=="CardPool_Content")
            continue;
        if (str=="ContentEnd")
            break;
        if (str=="PeaShooter_Card"){
            //必须先加载后创建，否则sizefactor无法初始化
            PeaShooter::LoadTexture();
            plantPtr= new PeaShooter;
            // plantPtr->LoadTexture();
            Card<PeaShooter>* card = new Card<PeaShooter>;
            std::cout<<" ============== "<<((PeaShooter*)plantPtr)->SizeFactor<<std::endl;
            card->Init(plantPtr->getFrameStartInd(),plantPtr->getCost(),plantPtr->getCoolDown(),((PeaShooter*)plantPtr)->SizeFactor,"PeaShooter");
            card->LoadInstance();
            insert(card);
        }
        if (str=="SunFlower_Card"){
            SunFlower::LoadTexture();
            plantPtr = new SunFlower;
            // plantPtr->LoadTexture();//顺便加载
            Card<SunFlower>* card = new Card<SunFlower>;
            card->Init(plantPtr->getFrameStartInd(),plantPtr->getCost(),plantPtr->getCoolDown(),((SunFlower*)plantPtr)->SizeFactor,"SunFlower");
            card->LoadInstance();
            insert(card);
        }
        if (str == "SnowPea_Card") {
            SnowPeaShooter::LoadTexture();
            plantPtr = new SnowPeaShooter;
            // plantPtr->LoadTexture();
            Card<SnowPeaShooter>* card = new Card<SnowPeaShooter>;
            card->Init(plantPtr->getFrameStartInd(), plantPtr->getCost(), plantPtr->getCoolDown(), ((SnowPeaShooter*)plantPtr)->SizeFactor, "SnowPea");
            card->LoadInstance();
            insert(card);
        }
        if (str == "Repeater_Card") {
            Repeater::LoadTexture();
            plantPtr = new Repeater;
            // plantPtr->LoadTexture();
            Card<Repeater>* card = new Card<Repeater>;
            card->Init(plantPtr->getFrameStartInd(), plantPtr->getCost(), plantPtr->getCoolDown(), ((Repeater*)plantPtr)->SizeFactor, "Repeater");
            card->LoadInstance();
            insert(card);
        }
        if (str == "WallNut_Card") {
            WallNut::LoadTexture();
            plantPtr = new WallNut;
            // plantPtr->LoadTexture();
            Card<WallNut>* card = new Card<WallNut>;
            card->Init(plantPtr->getFrameStartInd(), plantPtr->getCost(), plantPtr->getCoolDown(), ((WallNut*)plantPtr)->SizeFactor, "WallNut");
            card->LoadInstance();
            insert(card);
        }
    }
}
void SunPool::save(){
    std::ofstream outFile;
    outFile.open(ParentPath+SavePath,std::ios::app);
    if (!outFile.is_open()){
        std::cout<<"SunPool"<<line<<" open err\n";
    }
    outFile<<"SunPool "<<short(line)<<std::endl;
    node* t = Head;
    while(t){
        
        if (!t->item->SaveInstance()){
            std::cout<<"===================BasicPool save err=========================\n";
            return;
        }
        t=t->next;
    }
    outFile<<"ContentEnd"<<std::endl;
}
void SunPool::load(){/////这样设计load要调用6次，但是可以无视line顺序
    std::string str;
    LoadReader>>str;
    std::cout<<str;

    short templine;
    if (str=="SunPool"){
        LoadReader>>templine;
        std::cout<<templine<<std::endl;
    }else{
        return;
    }
    while(LoadReader>>str){
        std::cout<<str<<std::endl;
        if (str=="ContentEnd"){
            return;
        }
        if (str=="Sun"){
            Sun* sun=new Sun;
            sun->LoadInstance();
            sunPool[templine].insert(sun);
        }
    }
}
