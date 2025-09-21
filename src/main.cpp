#include "include/GameLoop.hpp"
#include <chrono>
#include <thread>

//获取当前工作目录的父路径
std::string ParentPath;

std::optional<sf::Event> WindowEvent;//全局窗口事件

//声明全局变量用于主循环
Dialog dialog_exit(sf::Vector2u({120,90}),0.3,50,50);
Dialog_Save dialog_save(sf::Vector2u({80,60}),0.2,50,50);
Dialog_Restart dialog_restart(sf::Vector2u({80,60}),0.2,50,50);
PauseButton pauseButton;
ScreenAdventure screenAdventure;

int main() {
    ParentPath = std::filesystem::current_path().parent_path().string();//获取当前工作目录的父路径
    
    auto target_frame_duration = std::chrono::microseconds(1000000UL / TickPerSec);//目标帧持续时间，单位为微秒

//一些局部变量的初始化
    TICK LoseAnimationTickRemain = 0;//失败动画剩余刻数
    bool clearOnce = 0;//清除一次标志，防止重复清除
    bool loadOnce = 0;//加载一次标志，防止重复加载
   
    Init();//初始化窗口

    //主循环
    while (window.isOpen()){
        auto prev_time = std::chrono::high_resolution_clock::now();//获取上一帧时间
        
        WindowEvent = std::nullopt;//清空事件
        windowSize = window.getSize();//获取窗口大小
        while (WindowEvent = window.pollEvent()){//处理事件
            if (WindowEvent->is<sf::Event::Closed>()) {//窗口关闭事件
                window.close();
            }
            if (window.hasFocus())//如果窗口有焦点
                if(const auto* keyPressed = WindowEvent->getIf<sf::Event::KeyPressed>()){//键盘按下Esc按键事件
                    if (keyPressed->scancode==sf::Keyboard::Scancode::Escape)
                        window.close();
                }
            myMouse.update();//更新鼠标状态，必须在事件处理后面，否则无法获取鼠标位置
        }
        
        window.clear(sf::Color::Black);//清除窗口，设置背景色为黑色
            
        if (GameActivated){
            if (!clearOnce){
                Clear();
                clearOnce = 1;
                LoseAnimationTickRemain = 0;
            }
            if (!loadOnce){
                loadOnce = 1;
                if (UIstate.Restart()){//检测是否重启游戏
                    Load(1);//////reset
                }else{
                    Load(0);
                }
            }
            sf::Sprite BG(Background);
            BG.setPosition({CollisionDatas::MAPxTranslation,0});
            window.draw(BG);
            //////先画背景 再启动gameloop
            GameLoop();

            //////失败动画
            if (GameLosed && LoseAnimationTickRemain == 0){//重置动画刻数
                LoseAnimationTickRemain = 1.5 * TickPerSec;
            }
            if (GameLosed && LoseAnimationTickRemain!=0){
                sf::Sprite lose(Lose);
                lose.setPosition({20,0});
                float fac = (1.5 * TickPerSec - LoseAnimationTickRemain)/(1.5 *TickPerSec) + 0.3;//这个秒数必须同步zombie game'lose
                if (fac>0.8)fac = 0.8;
                lose.setScale({fac,fac});
                window.draw(lose);
                LoseAnimationTickRemain--;
            }
            //UI状态更新
            pauseButton.update();
            dialog_exit.update();
            dialog_save.update();
        }else{
            //重置标记
            clearOnce = 0;
            loadOnce = 0;
            sf::Sprite SF(Surface);
            window.draw(SF);

            if (UIstate.Save()){
                std::cout<<"Save in main"<<std::endl;
                Save();
            }
            //首页UI更新
            screenAdventure.update();
            dialog_restart.update();
        }

    
        window.display();//渲染窗口内容
        if (!GamePaused){
            TickUpdate();//更新全局游戏刻
        }

///////////////计算程序需要睡眠的时间
        auto current_time = std::chrono::high_resolution_clock::now();///获取当前时间
        auto sleep = std::chrono::duration_cast<std::chrono::microseconds>(
            target_frame_duration - (current_time - prev_time) 
        ).count();
        if (std::chrono::microseconds(sleep) > std::chrono::microseconds(0))//溢出保护
            std::this_thread::sleep_for(std::chrono::microseconds(sleep));

    }
    return EXIT_SUCCESS;
}