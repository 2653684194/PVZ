#ifndef SUNPOOL_HPP
#define SUNPOOL_HPP


#include "Instances.hpp"

void SunPool::update(){

    node* temp=Head,*pre=nullptr;
    while(temp){
        Sun* item=temp->item;
        item->update();
        node* tempnext=temp->next;

        // std::cout<<"SunPoolIs updated\n";
        if (myMouse.LClickProtection&&myMouse.LButtonPressed){
            // std::cout<<"isclicked\n";
            if (item->isclicked())
                myMouse.LClickProtection=0;
        }
        if (item->isdie()){
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






















/////////////////////////////////////////////像纪念死者一样纪念这串代码////////////////////////////////////

//采用阳光分区设计
// template<class instance>
// class SunPool{
// public:
//     struct node{
//         instance* item;
//         node* next;
//         node(instance* i):item(i),next(nullptr){}
//     };
//     class List{
//     public:
//         float xStart,xEnd,yStart,yEnd;
//         node* Head;
//         // node** cur;
//         List(float xS,float xE,float yS,float yE):xStart(xS),xEnd(xE),yStart(yS),yEnd(yE){
//             Head=nullptr;
//             cur=&Head;
//         }
//         ~List(){
//             node*temp;
//             while (Head){
//                 temp=Head;
//                 Head=Head->next;
//                 delete temp;
//             }
//         }
//         void insert(instance* item){
//             if (!Head){
//                 Head=newnode;
//                 return;
//             }
//             node** cur=&Head;
//             while((*cur)->next)
//                 cur=&((*cur)->next);
//             (*cur)->next=new node(item);
//             // cur=&(newnode);
//         }
//         void clickupdate(){
//             node* temp=Head;
//             while(temp){
//                 temp->item->isclicked();
//                 temp=temp->next;
//             }
//         }
//     };
// private:
//     const short xDivision = 10;
//     const short yDivision = 5;
//     List** subarea;

// public:
//     SunPool(){
//         subarea = new List*[xDivision*yDivision];
//         for (short x=0;x<xDivision;x++){
//             for (short y=0;y<yDivision;y++){
//                 subarea[x+y*xDivision]=new List(x*WINDOW_WIDTH/xDivision,(x+1)*WINDOW_WIDTH/xDivision,y*WINDOW_HEIGHT/yDivision,(y+1)*WINDOW_HEIGHT/yDivision);
//                 //坐标映射看鼠标坐标映射方式
//                 std::cout<<x*WINDOW_WIDTH/xDivision<<" "<<(x+1)*WINDOW_WIDTH/xDivision<<" "<<y*WINDOW_HEIGHT/yDivision<<" "<<(y+1)*WINDOW_HEIGHT/yDivision<<"\n";

//             }
//         }
//     }
//     ~SunPool(){
//         for (short x=0;x<xDivision;x++){
//             for (short y=0;y<yDivision;y++){
//                 subarea[x+y*xDivision]->~List();
//             }
//         }
//     }

//     void insert(instance* item){
//         std::cout<<int(item->getx()/WINDOW_WIDTH*xDivision)<<" "<<int(item->gety()/WINDOW_HEIGHT*yDivision)<<"\n";
//         subarea[int(item->getx()/WINDOW_WIDTH*xDivision)+int(item->gety()/WINDOW_HEIGHT*yDivision)*xDivision]->insert(item);
//     }
//     void erase(instance* item);
//     void update(){
//         List* temp;
//         node* t,*pre;
//         for (short x=0;x<xDivision;x++){
//             for (short y=0;y<yDivision;y++){
//                 temp=subarea[x+y*xDivision];
//                 t=temp->Head;
//                 if (!t)
//                     continue;
//                 pre=nullptr;  

//                 if ((mousePosition.x >= temp->xStart&&mousePosition.x < temp->xEnd&&(mousePosition.y) >= temp->yStart&&mousePosition.y < temp->yEnd)){
                    
//                     if (MouseIsEnter&&window.hasFocus()&&(sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))){
//                         // std::cout<<"Mouse clicked detect in Sunpool"<<temp->xStart<<","<<temp->yStart<<"\n";
//                         temp->clickupdate();
//                     }
//                 }

//                 short i=x,j=y;
//                 while(t){
//                     instance* item=t->item;
//                     node* tnext=t->next;
//                     item->update();
//                     if (item->isdie()){
//                         std::cout<<"Sun is die\n";
                        
//                         delete item;
//                         if (pre){
//                             pre->next=t->next;
//                         }else{
//                             temp->Head=temp->Head->next;
//                         }
//                         delete t;
//                         t=tnext;
//                         continue;
//                     }
//                     if (item->getx()<temp->xStart)
//                         i--;
//                     else if (item->getx()>=temp->xEnd)
//                         i++;
//                     if (item->gety()<temp->yStart)
//                         j--;
//                     else if (item->gety()>=temp->yEnd)
//                         j++;
//                     if (i-x||j-y){
//                         node* tt=t->next;
//                         if (i>=0&&i<xDivision&&j>=0&&j<yDivision){
//                             subarea[i+j*xDivision]->insert(item);
//                             std::cout<<item->getx()<<","<<item->gety()<<"was transfer\n";
//                         }else{//飞出边界
//                             delete item;
//                         }
//                         if (pre){
//                             pre->next=t->next;
//                             delete t;//不可以删除内容
//                         }else{
//                             temp->Head=temp->Head->next;
//                             delete t;
//                         }
//                         t=tnext;
//                         continue;
//                     }
//                     pre=t;
//                     t=tnext;
//                 }
        
//             }
//         }
//     }
    
// };
#endif