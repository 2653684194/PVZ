#ifndef BULLETPOOL_HPP
#define BULLETPOOL_HPP

#include "Bullet.hpp"
template <class instance>
BulletPool<instance>::~BulletPool(){
    while (Head){
        node* temp=Head;
        Head=Head->next;
        delete temp->item;//后续换成temp->die()
        delete temp;
    }
}


template <class instance>
void BulletPool<instance>::swap(node* a,node* b){//注意是指针的引用
    if (!a||!b||a==b)
        return;
    instance* temp=a->item;
    a->item=b->item;
    b->item=temp;
}


template <class instance>
void BulletPool<instance>::insert(instance* item) {//x 大在前
    node* newNode = new node(item);
    if (!Head || item->getx() >= Head->item->getx()) {
        newNode->next = Head;
        if (Head) Head->pre = newNode;
        Head = newNode;
        return;
    }
    node* cur = Head;
    while (cur->next && item->getx() < cur->next->item->getx()) {
        cur = cur->next;
    }
    newNode->next = cur->next;
    if (cur->next) cur->next->pre = newNode;
    cur->next = newNode;
    newNode->pre = cur;
    // Prework(bulletPool[line],zombiePool[line]);
}

template <class T>
inline HP* BulletPool<T>::GetZombieHP(Zombie* zb){return &(zb->hp);}
template <class T>
inline void BulletPool<T>::SlowDownActivated(Zombie* zb){zb->isslowdown = 1;}
template <class T>
inline DG BulletPool<T>::GetBulletDG(Bullet* bl){return bl->damage;}



const COORD CollideRange = 4;
template <typename T1,typename T2>
void BPInteractWithZP(BulletPool<T1>& bulletPool, ZombiePool<T2>& zombiePool){
    if (!bulletPool.Fronts.empty())
        // std::cout<<std::endl;
    for (short i=0;i<bulletPool.Fronts.size();i++){
        Zombie* zombie=zombiePool.Fronts[i]->item;
        Bullet* bullet=bulletPool.Fronts[i]->item;
        // std::cout<<"zommbie:"<<zombie->getx()<<"\tbullet:"<<bullet->getx()<<std::endl;
        COORD gap=zombie->getx() + CollisionDatas::ZombieSizeX / CollisionDatas::factor * 0.1 - bullet->getx();//考虑僵尸背部的比较
        if (gap<CollideRange){//如果子弹的x小于僵尸的x+僵尸宽度
            if (bullet->die(window)){

                HP* hp=bulletPool.GetZombieHP(zombie);
                *hp -= bulletPool.GetBulletDG(bullet);
                if (bullet->isIced()&& *hp>0){
                    bulletPool.SlowDownActivated(zombie);
                }
                auto cur = bulletPool.Fronts[i];
                auto next = cur->next;
                auto pre = cur->pre;
                if (pre) {
                    pre->next = next;
                    if (next) next->pre = pre;
                } else {
                    bulletPool.Head = next;
                    if (next) next->pre = nullptr;
                }
                delete cur->item;
                delete cur;
                // Prework(bulletPool,zombiePool);
                continue;//!!!
            }
        }
    }
}


template <class instance>
void BulletPool<instance>::update(RWD& wd) {    
    Prework(bulletPool[line],zombiePool[line]);//在ZombiePool定义
    BPInteractWithZP(bulletPool[line],zombiePool[line]);

    node *pre = nullptr, *cur = Head;//声明要放后面
    while (cur) {
        // std::cout << "update\n";
        COORD x = cur->item->getx();
        
        // 检查是否需要移除节点
        if (x > WINDOW_WIDTH || x < 0) {
            if (cur->item->die(wd)){
                node* next = cur->next;
                if (pre) {
                    pre->next = next;
                    if (next) next->pre = pre;
                } else {
                    Head = next;
                    if (next) next->pre = nullptr;
                }
                delete cur->item;
                delete cur;
                cur = next;
                // Prework(bulletPool[line],zombiePool[line]);
                continue;//!!!
            }
        }else{
            // 更新节点
            cur->item->update(wd);
            // 排序逻辑 - 需要确保交换后指针正确
            node* sortcur = cur;
            while (sortcur->pre && sortcur->item->getx() > sortcur->pre->item->getx()) {
                swap(cur,sortcur->pre);
                sortcur=sortcur->pre;
            }
        }
        // 正常前进
        pre = cur;
        cur = cur->next;
    }
    //顺序检查
    // cur=Head;
    // while(cur){
    //     std::cout<<"instance in BulletPool: "<<cur->item->getx()<<std::endl;
    //     cur=cur->next;
    // }
}


template <class instance>
void BulletPool<instance>::clear(){
    while (Head){
        node* temp=Head;
        Head=Head->next;
        delete temp->item;
        delete temp;
    }
}

template <class instance>
void BulletPool<instance>::save(){
    std::ofstream outFile;
    outFile.open(ParentPath+SavePath,std::ios::app);
    if (!outFile.is_open()){
        std::cout<<"BulletPool"<<short(line)<<" open outFile err\n";
    }
    outFile<<"BulletPool "<<short(line)<<std::endl;
    node* t = Head;
    while(t){
        if (!t->item->SaveInstance()){
            std::cout<<"===================BulltePool save err=========================\n";
            return;
        }
        t=t->next;
    }
    outFile<<"ContentEnd"<<std::endl;
}
template <class instance>
void BulletPool<instance>::load(){
    ///////防止重复load可以调用clear()/////////
    std::string str;
    LoadReader>>str;
    short templine;
    if (str=="BulletPool"){
        LoadReader>>templine;
        std::cout<<"BulletPool "<<templine<<std::endl;
    }else{
        return;
    }
    while(LoadReader>>str){
        if (str=="ContentEnd"){
            std::cout<<"ContentEnd\n";
            return;
        }
        Bullet* blt;
        if (str=="Pea"){
            blt = new Pea;
            blt->LoadInstance();
            bulletPool[templine].insert(blt);
        }
    }
}




#endif