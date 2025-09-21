#ifndef UI_HPP
#define UI_HPP


#include "Instances.hpp"

class PauseButton;
class Dialog;
class Dialog_Save;
class Dialog_Restart;
class ScreenAdventure;
class UIState;


class UIState{
public:
    enum State{
        hidden=0,
        display=1,
    };
private:
    State PauseButtonState;
    State Dialog_ExitState;
    State Dialog_SaveState;
    State Dialog_RestartState;
    State ScreenAdventureState;
    friend class PauseButton;
    friend class Dialog;
    friend class Dialog_Save;
    friend class Dialog_Restart;
    friend class ScreenAdventure;
    friend class Zombie;
    
    bool isSave;
    bool isRestart;

public:
    UIState():Dialog_ExitState(State(0)),Dialog_SaveState(State(0)),Dialog_RestartState(State(0)),PauseButtonState(State(1)),ScreenAdventureState(State(0)),isSave(0),isRestart(0){}
    inline bool Save(){
        bool IS = isSave;
        isSave = 0;
        return IS;
    }
    inline bool Restart(){
        bool IR = isRestart;
        isRestart = 0;//有效避免在main函数定义保护哨兵
        return IR;
    }
};
extern UIState UIstate;


class Dialog{

protected:
    COORD x,y;
    sf::Vector2u CenterSize;
    _Float16 FrameWidthSizeFactor;

    sf::Vector2f ContinueRelativePos;
    sf::Vector2u ButtonSize;
    _Float16 ButtonSizeFactor;

    _Float16 FontSizeFactor;

    sf::Vector2f ExitRelativePos;

    static std::string DialogImgPath;
    sf::Texture DialogTextures[10];
    sf::Texture DialogTexture;
    sf::Texture ContinueTexture;
    sf::Texture ExitTexture;

    sf::Vector2u DialogSize;

public:
friend class PauseButton;
    Dialog(sf::Vector2u CS,float FW,COORD xx,COORD yy ,float fsf = 0.6);
    void Init(bool Header=1,std::string text1 = "continue",std::string text2 = "exit",std::string sentence = " ");
    void update(UIState& uis=UIstate);

};
extern Dialog dialog_exit;

class Dialog_Save:public Dialog{
public:
    friend class PauseButton;
    Dialog_Save(sf::Vector2u CS,float FW,COORD xx,COORD yy ,float FontSizeFactor = 0.4):Dialog(CS,FW,xx,yy,FontSizeFactor){}
    void update(UIState& uis=UIstate);

};
extern Dialog_Save dialog_save;

class Dialog_Restart:public Dialog{
public:
    friend class PauseButton;
    Dialog_Restart(sf::Vector2u CS,float FW,COORD xx,COORD yy , float FontSizeFactor = 0.25):Dialog(CS,FW,xx,yy,FontSizeFactor){}
    void update(UIState& uis=UIstate);

};
extern Dialog_Restart dialog_restart;


class PauseButton{
private:

    static std::string ImgPath;
    sf::Texture texture;
    COORD x,y;
    COORD Sizex,Sizey;
    _Float16 SizeFactor;

public:
    PauseButton(COORD xx=400,COORD yy=3);
    void Init();
    void update(UIState& uis=UIstate);

};
extern PauseButton pauseButton;

class ScreenAdventure{
public:
    enum State{
        Origin = 0,
        Start = 1
    };
private:
    State state;
    const std::string ImgPath = "/img/UI/Menu/SelectorScreenAdventure.png",StartImgPath = "/img/UI/Menu/SelectorScreenStartAdventure.png";
    sf::Texture Texture,StartTexture;
    sf::Vector2u Pos;
    // sf::Vector2u UpLeft,UpRight,DownLeft,DownRight;
    sf::Vector2f Points[4];
    const float SizeFactor = 0.65;
    const float SecToGetIn = 0.55;
    TICK TickToGetIn;
    bool IsInPolygon;
public:
    ScreenAdventure(COORD x = 237,COORD y = 15);
    void Init();
    void Reset();
    void update(UIState& uis=UIstate);
};
extern ScreenAdventure screenAdventure;







#endif