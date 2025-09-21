#ifndef CARDPOOL_HPP
#define CARDPOOL_HPP

#include "Instances.hpp"

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





#endif