#include "include/Instances.hpp"

void MyMouse::update(){
    if (WindowEvent->getIf<sf::Event::MouseEntered>()){
        std::cout<<"Mouse Enter\n";
        MouseIsEnter=1;
    }else if (WindowEvent->getIf<sf::Event::MouseLeft>()){
        MouseIsEnter=0;
        LClickProtection=1;
        LButtonPressed=0;
        RClickProtection=1;
        RButtonPressed=0;
    }
    if (MouseIsEnter&&window.hasFocus()){
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
            LButtonPressed=1;
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)){
            RButtonPressed=1;
        }

        if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
            LClickProtection=1;
            LButtonPressed=0;
        }
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)){
            RClickProtection=1;
            RButtonPressed=0;
        }
    }
        MousePosition = sf::Mouse::getPosition(window);//位置更新;
        MousePosition.x=(MousePosition.x*WINDOW_WIDTH*1.0/windowSize.x);//窗口投射 
        MousePosition.y=(MousePosition.y*WINDOW_HEIGHT*1.0/windowSize.y);

        MouseInGridX=(MousePosition.x-CollisionDatas::PlantxInc-CollisionDatas::MAPxTranslation)*1.0/CollisionDatas::xcast;
        MouseInGridY=(MousePosition.y-CollisionDatas::PlantyInc)*1.0/CollisionDatas::ycast;
}
