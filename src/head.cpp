#include "include/Head.hpp"
RWD window(sf::VideoMode({WINDOW_WIDTH,WINDOW_HEIGHT}),"PVZ",sf::Style::Default);

sf::Vector2u windowSize;

bool GameActivated=0;
bool GamePaused = 0;
bool GameLosed = 0;

TICK GlobalTick=0;
TICK TickPerSec=90;
TICK TimeLoop=100;


COORD MAPxTranslation = -70;//地图平移  

COORD CollisionDatas::MAPxTranslation = -70;

const float CollisionDatas::factor = 25;
const  COORD CollisionDatas::xcast = 43,CollisionDatas::ycast = 55;
const COORD CollisionDatas::PlantxInc = 133,CollisionDatas::PlantyInc = 43;
const COORD CollisionDatas::ZombieyInc = 4;
const COORD CollisionDatas::PeaxInc = 16.2;
const COORD CollisionDatas::PlantSizeX = 43.8 * CollisionDatas::factor;//43.8 * factor 
const COORD CollisionDatas::ZombieSizeX = 47.4 * CollisionDatas::factor, CollisionDatas::ZombieSizeY = 86.4;
const COORD CollisionDatas::PeaSizeX = 16.2;
const COORD CollisionDatas::PeaSizeY = 20.4;



std::string SavePath = "/save/username_save1.txt";
std::ifstream LoadReader(ParentPath+SavePath);

void TickUpdate(){
    GlobalTick=(GlobalTick+1)%(TickPerSec*TimeLoop);
}
char bkpoint = '\0';
void breakpoint(){
    std::cin>>bkpoint;
}
bool CubePercentage(int denominator,int numerator){
    if (denominator==0)
        std::cout<<"denominator==0\n";
    std::srand(std::time(nullptr)+rand());
    double fraction=1.0*numerator/denominator;
    return std::rand()%denominator<=denominator*fraction*fraction*fraction;
}

std::vector<sf::Texture> GlobalDynamicTextures(0);//头文件合声明cpp初始化
//在每个类内调用该辅助函数，main函数声明类然后预加载再进入游戏循环，类的对应静态变量索引初始化为-1（标记） 后续可能考虑使用unsigned管理
short LoadTexture(std::string ImgPath,short frame,float SizeFactor){//返回开始索引，方便处理，返回-1表示加载失败
    short startind=GlobalDynamicTextures.size();
    std::cout<<"Vector size:"<<startind<<std::endl;
    for (short i=1;i<=frame;i++){
        sf::Texture t;
        if (!t.loadFromFile(ParentPath+ImgPath+std::to_string(i)+".png")){
            std::cout<<"load img err"<<std::endl;
            return -1;
        }
        GlobalDynamicTextures.push_back(t);
    }
    std::cout<<startind<<std::endl;
    return startind;
};

