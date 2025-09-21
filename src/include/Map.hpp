#ifndef MAP_HPP
#define MAP_HPP

#include "Plant.hpp"
template <class instance>
uint8_t Map<instance>::PlantedNum = 0;
template <class instance>
Map<instance>::Map(){//初始化地图
    for (short i=0;i<MAP_LENGTH;i++){
        for (short j=0;j<DAY_MAP_WIDTH;j++)
            map[i][j]=nullptr;
    } 
}
template <class instance>
bool Map<instance>::insert(instance* item,short x,short y){
    if (map[x][y]||!item||x<0||x>=MAP_LENGTH||y<0||y>=DAY_MAP_WIDTH)
        return false;
    else   
        map[x][y]=item;
    return true;
}
template <class instance>
bool Map<instance>::erase(short x,short y){
    if (!map[x][y]){
        return false;
    }else{
        delete map[x][y];
        map[x][y]=nullptr;
        return true;
    }
}

template <class instance>
uint8_t Map<instance>::NumOfPlants(){return PlantedNum;}

template <class instance>
void Map<instance>::update(sf::RenderWindow& wd,uint16_t j){
    if (j==0){
        PlantedNum = 0;
    }
    // for (short j=0;j<DAY_MAP_WIDTH;j++){
        for (short i=0;i<MAP_LENGTH;i++){    
            // std::cout<<"check at "<<i<<","<<j<<std::endl;
            if (map[i][j]){//在僵尸处好像erase了所以没有必要  &&*(map[i][j]->getHP())>0
                map[i][j]->show(wd);
                map[i][j]->action();
                PlantedNum++;
            }
        }
        // sunPool[j].update();//阳光挡住当前植物
    // }
}
template <class instance>
instance* Map<instance>::at(short x,short y){
    if (x>=0&&x<MAP_LENGTH&&y>=0&&y<DAY_MAP_WIDTH)
        return map[x][y];
    return nullptr;
}

template <class instance>
void Map<instance>::clear(){
    for (short i=0;i<MAP_LENGTH;i++){
        for (short j=0;j<DAY_MAP_WIDTH;j++){
            if (map[i][j])
                delete map[i][j];
            map[i][j]=nullptr;
        }
    }
}
template <class instance>
void Map<instance>::save(){

    std::ofstream outFile;
    outFile.open(ParentPath+SavePath,std::ios::app);
    if (!outFile.is_open()){
        std::cout<<"Map open outFile err\n";
    }
    outFile<<"Map"<<std::endl;
    for (short i=0;i<MAP_LENGTH;i++){
        for (short j=0;j<DAY_MAP_WIDTH;j++){
            if (map[i][j]&&!map[i][j]->SaveInstance()){
                std::cout<<"↑================Map save err====================\n";
                return;
            }
        }
    }
    outFile<<"ContentEnd"<<std::endl;
}
template <class instance>
void Map<instance>::load(){
    std::string str;
    LoadReader>>str;
    if (str!="Map"){
        return;
    }
    std::cout<<"Map"<<std::endl;
    short x,y;
    Plant* plt;
    while(LoadReader>>str){
        std::cout<<str<<std::endl;
        if (str=="ContentEnd"){
            std::cout<<"ContentEnd\n";
            return;
        }
        LoadReader>>x>>y;

        if (str=="PeaShooter"){
            plt = new PeaShooter(x,y);
        }
        if (str=="SunFlower"){
            plt = new SunFlower(x,y);
        }
        if (str == "SnowPea") {
            plt = new SnowPeaShooter(x, y);
        }
        if (str == "Repeater") {
            plt = new Repeater(x, y);
        }
        if (str == "WallNut") {
            plt = new WallNut(x, y);
        }
        if (plt->LoadInstance()){///由于缺乏if保护，请务必检查植物名一一对应
            ;
        }
        insert(plt,x,y);///由于缺乏if保护，请务必检查植物名一一对应
    }
}

#endif