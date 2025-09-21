#ifndef HEAD_HPP
#define HEAD_HPP

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

//用于获取当期工作目录上级路径，这是.exe放在bin文件的设计
#include <filesystem>

//用于存档
#include <fstream>

//用于随机数
#include <cstdlib>
#include <ctime>


using HP = float;//Hit Point
using TICK = int16_t;//游戏刻，必须包含负数，否则出错
using DG = float;//damage
using COORD = short;//坐标 coordinates

// using FCOORD = float;//float coordinates
using SPD = float;//speed
using COST = uint16_t;//植物消耗的阳光

using RWD = sf::RenderWindow;
using FileStr = std::filesystem::__cxx11::path;
extern std::string ParentPath;//父路径，获取当前工作目录的父路径

extern std::optional<sf::Event> WindowEvent;//全局窗口事件

extern RWD window;//全局窗口

extern sf::Vector2u windowSize;//全局窗口大小

extern bool GameActivated;//游戏是否激活
extern bool GamePaused;//游戏是否暂停
extern bool GameLosed;//游戏是否失败

extern sf::Texture Lose;//失败画面


extern TICK TickPerSec;//每秒游戏刻数
extern TICK GlobalTick;//全局游戏刻
extern TICK TimeLoop;//游戏循环时间，单位为秒
void TickUpdate();//更新全局游戏刻
bool CubePercentage(int denominator,int numerator);//次方概率，分母为0时返回false

//碰撞检测相关数据
//所有坐标都以左上角为原点，x轴向右，y轴向下
struct CollisionDatas{
    const static float factor;//缩放因子，提高某些坐标精度
    static COORD MAPxTranslation;//地图平移，地图左上角在屏幕左上角的x轴位置
    const static COORD xcast , ycast;//碰撞检测用的x轴和y轴偏移量，单位为像素
    const static COORD PlantxInc , PlantyInc;//植物的x轴和y轴偏移量，单位为像素
    const static COORD ZombieyInc;//僵尸的y轴偏移量，单位为像素
    const static COORD PeaxInc;
    //规定了大小，要求所有实例都符合
    const static COORD PlantSizeX;//植物的x轴大小
    const static COORD ZombieSizeX , ZombieSizeY;//僵尸的x轴和y轴大小
    const static COORD PeaSizeX,PeaSizeY;//豌豆子弹的x轴和y轴大小
};

const short WINDOW_WIDTH = 480;//窗口宽度
const short WINDOW_HEIGHT = 320;//窗口高度

extern std::vector<sf::Texture> GlobalDynamicTextures;//全局动态纹理，预加载的纹理存储在这里
//在每个类内调用该辅助函数，main函数声明类然后预加载再进入游戏循环，类的对应静态变量索引初始化为-1（标记） 后续可能考虑使用unsigned管理
short LoadTexture(std::string ImgPath,short frame,float SizeFactor = 1.0);//返回开始索引，方便处理，返回-1表示加载失败

extern char bkpoint;//全局断点变量，调试用
void breakpoint();//调试用断点函数

extern std::ifstream LoadReader;//全局加载器，读取存档文件
extern std::string SavePath;//全局存档路径，存档文件名为username_save1.txt
#endif