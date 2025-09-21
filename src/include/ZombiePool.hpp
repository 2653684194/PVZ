#ifndef ZOMBIEPOOL_HPP
#define ZOMBIEPOOL_HPP

#include "Instances.hpp"

template <class instance>
void ZombiePool<instance>::swap(node* a,node* b){//注意是指针的引用,swap改用内容swap
    if (!a||!b||a==b)
        return;
    instance* temp=a->item;
    a->item=b->item;
    b->item=temp;
}
template <class instance>
ZombiePool<instance>::~ZombiePool(){
    while (Head){
        node* temp=Head;
        Head=Head->next;
        delete temp->item;//后续换成temp->die()
        delete temp;
    }
}

template <class instance>
void ZombiePool<instance>::insert(instance* item) {//x 大在前
    node* newNode = new node(item);
    // if (Head!=nullptr){
    //     std::cout<<"Head is not nullptr\n";
    // }
    if (!Head || item->getx() <= Head->item->getx()) {//如果是第一个节点或者新节点的x小于等于头节点的x
        newNode->next = Head;// 将新节点的next指向当前头节点
        if (Head) Head->pre = newNode;// 如果头节点存在，则更新其前驱指针
        Head = newNode;// 将新节点设置为头节点
        return;
    }

    node* cur = Head;
    while (cur->next && item->getx() > cur->next->item->getx()) {//找到合适的位置
        cur = cur->next;
    }
    newNode->next = cur->next;// 插入到cur之后
    if (cur->next) cur->next->pre = newNode;// 如果cur之后有节点，则更新其前驱指针
    cur->next = newNode;// 将新节点插入到cur之后
    newNode->pre = cur;// 更新新节点的前驱指针
    // Prework(bulletPool[line],zombiePool[line]);
}

template <class instance>
void ZombiePool<instance>::update(RWD& wd) {
    node *pre = nullptr, *cur = Head;
    while (cur) {
        // std::cout << "update\n";
        COORD x = cur->item->getx();
        
        // 检查是否需要移除节点
        if (cur->item->GetZombieHP()<=0) {
            // std::cout<<"zombie die\n";
            if (cur->item->die(wd)) {// 如果僵尸死亡
                //删除节点
                node* next = cur->next;
                if (pre) {
                    pre->next = next;
                    if (next) next->pre = pre;
                } else {
                    Head = next;
                    if (next) next->pre = nullptr;
                }
                delete cur->item;//清除实例
                delete cur;//清除节点
                cur = next;
                // Prework(bulletPool[line],zombiePool[line]);
                continue;//!!!
            }
        }else{
            // 更新节点
            cur->item->update(wd);
            if (cur->item->SwitchLane()){
                zombiePool[cur->item->getDestY()].insert(cur->item);

                node* next = cur->next;// 获取下一个节点
                // 删除当前节点
                if (pre) {
                    pre->next = next;
                    if (next) next->pre = pre;
                } else {
                    Head = next;
                    if (next) next->pre = nullptr;
                }
                // delete cur->item;//不可以delete，因为insert并没有创建新的僵尸，而是把僵尸传递
                delete cur;
                cur = next;//因为当前节点变成下一节点，所以可以直接continue 避免重复操作
                continue;//!!!
            }
            // 排序逻辑 - 需要确保交换后指针正确
            node* sortcur = cur;
            while (sortcur->pre && sortcur->item->getx() < sortcur->pre->item->getx()) {// 如果当前节点的x小于前一个节点的x
                swap(sortcur,sortcur->pre);
                sortcur=sortcur->pre;
            }
        }
        // 正常前进
        pre = cur;
        cur = cur->next;
    }
}

// // 插入或删除节点需要prework  // 处理时用僵尸的背部和  子弹的前部(较复杂 暂时不实现)  进行比较
template <class T_Bullet,class T_Zombie>
void Prework(BulletPool<T_Bullet> &BP,ZombiePool<T_Zombie> &ZP){
    auto Zcur = ZP.Head;  // 僵尸当前节点 (x小的在前)
    auto Bcur = BP.Head;  // 子弹当前节点 (x大的在前)
    
    BP.Fronts.clear();
    ZP.Fronts.clear();
    
    if (!Zcur || !Bcur) {//如果任一池为空
        BP.Fronts.shrink_to_fit();
        ZP.Fronts.shrink_to_fit();
        return;
    }

    COORD firstZBx = Zcur->item->getx()+CollisionDatas::ZombieSizeX;
    while(Bcur->item->getx()>firstZBx){//找到第一个子弹的x大于僵尸的x+僵尸宽度
        if (!Bcur->next)
            break;
        Bcur=Bcur->next;
    }
    if (Bcur->item->getx()<=firstZBx){//如果第一个子弹的x小于等于僵尸的x+僵尸宽度
        ZP.Fronts.push_back(Zcur);
        BP.Fronts.push_back(Bcur);
        Zcur=Zcur->next;
        Bcur=Bcur->pre;
    }
    while(Bcur&&Zcur){//遍历两个池
        if (Zcur->item->getx()+CollisionDatas::ZombieSizeX<Bcur->item->getx()){//如果僵尸的x+僵尸宽度小于子弹的x
            Zcur=Zcur->next;
        }else if(Bcur->pre&&Bcur->pre->item->getx()<=Zcur->item->getx()+CollisionDatas::ZombieSizeX){//如果子弹的前一个节点的x小于等于僵尸的x+僵尸宽度
            Bcur=Bcur->pre;
        }else{//如果子弹的x大于僵尸的x+僵尸宽度
            ZP.Fronts.push_back(Zcur);
            BP.Fronts.push_back(Bcur);
            Zcur=Zcur->next;
            Bcur=Bcur->pre;
        }
    }
}

template <class instance>
void ZombiePool<instance>::clear(){
    while (Head){
        node* temp=Head;
        Head=Head->next;
        delete temp->item;
        delete temp;
    }
}

template <class instance>
void ZombiePool<instance>::save(){

    std::ofstream outFile;
    outFile.open(ParentPath+SavePath,std::ios::app);
    if (!outFile.is_open()){
        std::cout<<"ZombiePool"<<int(line)<<" open outFile err\n";
    }
    outFile<<"ZombiePool "<<int(line)<<std::endl;//不能使用"\n"因为可能导致缓冲区不刷新使得输出顺序颠倒
    node* t = Head;
    while(t){
        if (!t->item->SaveInstance()){
            std::cout<<"===================ZombiePool save err=========================\n";
            return;
        }
        t=t->next;
    }
    outFile<<"ContentEnd"<<std::endl;
}
template <class instance>
void ZombiePool<instance>::load(){

    ///////防止重复load可以调用clear()/////////
    std::string str;
    short templine;
    LoadReader>>str;
    std::cout<<"Head: "<<str;
    if (str=="ZombiePool"){
        LoadReader>>templine;
        std::cout<<templine<<std::endl;
    }else{
        return;
    }
    while(LoadReader>>str){
        // std::cout<<std::endl<<"see what have got:"<<str<<std::endl;
        if (str=="ContentEnd"){
            return;
        }
        Zombie* zb;
        if (str=="RegularZombie"){
            zb = new RegularZombie;
            if (zb->LoadInstance())
                ;
            
            std::cout<<"zombiePool "<<templine<<" inserted"<<std::endl;
            zombiePool[templine].insert(zb);
        }
        else if (str == "ConeheadZombie") {
            zb = new ConeheadZombie;
            if (((ConeheadZombie*)zb)->LoadInstance())
                ;
            std::cout << "Conehead zombie inserted in line " << templine << std::endl;
            zombiePool[templine].insert(zb);
        }

    }
}

#endif