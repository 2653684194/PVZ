#include "include/UI.hpp"



class PauseButton;
UIState UIstate;



std::string Dialog::DialogImgPath = "/img/UI/Dialog/";


Dialog::Dialog(sf::Vector2u CS,float FW,COORD xx,COORD yy,float FSF):
    CenterSize(CS),FrameWidthSizeFactor(FW),x(xx),y(yy),FontSizeFactor(FSF){}

void Dialog::Init(bool Header,std::string text1,std::string text2,std::string sentence){

    if (!ContinueTexture.loadFromFile(ParentPath+DialogImgPath+"Button.png")){
        std::cout<<"Load Button err\n";
    }
    ExitTexture=ContinueTexture;

    if (!DialogTextures[0].loadFromFile(ParentPath+DialogImgPath+"dialog_header.png"))
        std::cout<<"Load DialogHeader err\n";
    for (short i=1;i<10;i++){
        if (!DialogTextures[i].loadFromFile(ParentPath+DialogImgPath+"dialog_"+std::to_string(i)+".png"))
            std::cout<<"Load dialog_"+std::to_string(i)+".png error\n";
    }
    sf::Vector2u FrameSize=DialogTextures[1].getSize();
    FrameSize.x*=FrameWidthSizeFactor,FrameSize.y*=FrameWidthSizeFactor;
    
///////////////////////////////////////
    ButtonSize=ExitTexture.getSize();
    ButtonSizeFactor = FrameSize.y*0.55/ButtonSize.y;


    sf::RenderTexture Continue({ButtonSize.x,ButtonSize.y}),Exit({ButtonSize.x,ButtonSize.y});
    
/////////////////////////////////字体/////////////////////////////////////////////////////
    sf::Font font;
    if (!font.openFromFile(ParentPath+"/font/arial.ttf")){
        std::cout<<"Failed to load font\n";
    }
    sf::Text ContinueText(font,text1,ButtonSize.y*0.6),ExitText(font,text2,ButtonSize.y*0.6),SentenceText(font,sentence,ButtonSize.y*FontSizeFactor);
    ContinueText.setFillColor(sf::Color::Green);
    ExitText.setFillColor(sf::Color::Green);
    SentenceText.setFillColor(sf::Color::Green);

    sf::Vector2f  ContinueSize= ContinueText.getGlobalBounds().size;
    sf::Vector2f  ExitSize= ExitText.getGlobalBounds().size;
   
    ContinueText.setPosition({(ButtonSize.x-ContinueSize.x)/2.0,(ButtonSize.y-ContinueSize.y)*0.333});
    ExitText.setPosition({(ButtonSize.x-ExitSize.x)/2.0,(ButtonSize.y-ExitSize.y)*0.333});
    
    sf::Sprite Button(ExitTexture);
    Continue.draw(Button);
    Exit.draw(Button);
    Continue.draw(ContinueText);
    Exit.draw(ExitText);
    Continue.display();
    Exit.display();
    ContinueTexture=Continue.getTexture();
    ExitTexture=Exit.getTexture();

    //必须放在后面调整，用于绘制
    ButtonSize.y = FrameSize.y*0.55;
    ButtonSize.x *=ButtonSizeFactor;


//////////////////////////////////////////////////////////////////

    sf::Vector2u HeaderSize=DialogTextures[0].getSize();
    float HeaderSizeFactor=CenterSize.x*1.0/HeaderSize.x;
    HeaderSize.y*=HeaderSizeFactor;

    ExitRelativePos.y=ContinueRelativePos.y = CenterSize.y + FrameSize.y + HeaderSize.y*0.8 + FrameSize.y*0.45/2;
    ContinueRelativePos.x = (CenterSize.x + 2*FrameSize.x)/3.0 - ExitTexture.getSize().x*ButtonSizeFactor/2; 
    ExitRelativePos.x = (CenterSize.x + 2*FrameSize.x)*2/3.0 - ExitTexture.getSize().x*ButtonSizeFactor/2; 
    
    sf::RenderTexture RT(sf::Vector2u({
        CenterSize.x + 2*FrameSize.x,
        CenterSize.y + 2*FrameSize.y + HeaderSize.y*0.8
    }));
    
    
    short xx = 0,yy = HeaderSize.y*0.8;
    for (short i =1;i<=3;i++){
        xx=0;
        for (short j=1;j<=3;j++){
            std::cout<<"(x,y): ("<<xx<<","<<yy<<")\n";

            sf::Sprite sp(DialogTextures[(i-1)*3+j]);
            if (i!=2&&j!=2)
                sp.setScale({FrameWidthSizeFactor,FrameWidthSizeFactor});////同步FrameSize
            if (i==2&&j==2){
                float xsf=CenterSize.x*1.0/DialogTextures[5].getSize().x;
                float ysf=CenterSize.y*1.0/DialogTextures[5].getSize().y;
                sp.setScale({xsf,ysf});
                SentenceText.setPosition({xx,yy});
                
            }
            if (i==2&&j!=2){

                float sf = CenterSize.y*1.0/(DialogTextures[(i-1)*3+j].getSize().y*FrameWidthSizeFactor);
                sp.setScale({FrameWidthSizeFactor,sf});
            }
            if (j==2&&i!=2){

                float sf = CenterSize.y*1.0/(DialogTextures[(i-1)*3+j].getSize().y*FrameWidthSizeFactor);
                sp.setScale({sf,FrameWidthSizeFactor});
            }
            
            sp.setPosition({xx,yy});
            RT.draw(sp);
            if (j==2){
                xx+=CenterSize.x;
            }else{
                xx+=FrameSize.x;
            }
        }
        if (i==2)
            yy+=CenterSize.y;
        else
            yy+=FrameSize.y;
    }
    RT.draw(SentenceText);
    if (Header){
        sf::Sprite sp(DialogTextures[0]);
        sp.setScale({HeaderSizeFactor,HeaderSizeFactor});
        sp.setPosition({FrameSize.x,0});
        RT.draw(sp);
    }
    RT.display();
    DialogTexture=RT.getTexture();
    DialogSize=DialogTexture.getSize();
}
void Dialog::update(UIState& uis){
    if (uis.Dialog_ExitState==UIState::State::display){
        sf::Sprite dl(DialogTexture);
        sf::Vector2f DialogPos((WINDOW_WIDTH-DialogSize.x)/2.0,(WINDOW_HEIGHT-DialogSize.y)/2.0);
        dl.setPosition(DialogPos);

        sf::Sprite continueButton(ContinueTexture);
        continueButton.setScale({ButtonSizeFactor,ButtonSizeFactor});
        continueButton.setPosition({
            ContinueRelativePos.x+DialogPos.x,
            ContinueRelativePos.y+DialogPos.y
        });
        sf::Sprite exitButton(ExitTexture);
        exitButton.setScale({ButtonSizeFactor,ButtonSizeFactor});
        exitButton.setPosition({
            ExitRelativePos.x+DialogPos.x,
            ExitRelativePos.y+DialogPos.y
        });


        sf::Color col=continueButton.getColor();
        col.r*=0.5;
        col.g*=0.5;
        col.b*=0.5;
        sf::Vector2i MousePos = myMouse.MousePosition;
        if (myMouse.LButtonPressed&&myMouse.LClickProtection&& 
            MousePos.x>=ContinueRelativePos.x+DialogPos.x&&MousePos.x<=ContinueRelativePos.x+DialogPos.x+ButtonSize.x&&
            MousePos.y>=ContinueRelativePos.y+DialogPos.y&&MousePos.y<=ContinueRelativePos.y+DialogPos.y+ButtonSize.y
        ){
            myMouse.LClickProtection=0;
            continueButton.setColor(col);
            GamePaused = 0;//////////////////////////////////////////continue
            uis.PauseButtonState = UIState::State::display;
            uis.Dialog_ExitState = UIState::State::hidden;

        }
        if (myMouse.LButtonPressed&&myMouse.LClickProtection&& 
            MousePos.x>=ExitRelativePos.x+DialogPos.x&&MousePos.x<=ExitRelativePos.x+DialogPos.x+ButtonSize.x&&
            MousePos.y>=ExitRelativePos.y+DialogPos.y&&MousePos.y<=ExitRelativePos.y+DialogPos.y+ButtonSize.y
        ){
            myMouse.LClickProtection=0;
            exitButton.setColor(col);
            ///////////////////////////////////////////exit
            uis.Dialog_SaveState = UIState::State::display;
            uis.Dialog_ExitState = UIState::State::hidden;
        }
        window.draw(dl);
        window.draw(continueButton);
        window.draw(exitButton);
    }
}

void Dialog_Save::update(UIState& uis){
    if (uis.Dialog_SaveState==UIState::State::display){
        sf::Sprite dl(DialogTexture);
        sf::Vector2f DialogPos((WINDOW_WIDTH-DialogSize.x)/2.0,(WINDOW_HEIGHT-DialogSize.y)/2.0);
        dl.setPosition(DialogPos);

        sf::Sprite yesButton(ContinueTexture);
        yesButton.setScale({ButtonSizeFactor,ButtonSizeFactor});
        yesButton.setPosition({
            ContinueRelativePos.x+DialogPos.x,
            ContinueRelativePos.y+DialogPos.y
        });
        sf::Sprite noButton(ExitTexture);
        noButton.setScale({ButtonSizeFactor,ButtonSizeFactor});
        noButton.setPosition({
            ExitRelativePos.x+DialogPos.x,
            ExitRelativePos.y+DialogPos.y
        });


        sf::Color col=yesButton.getColor();
        col.r*=0.5;
        col.g*=0.5;
        col.b*=0.5;
        sf::Vector2i MousePos = myMouse.MousePosition;
        if (myMouse.LButtonPressed&&myMouse.LClickProtection&& 
            MousePos.x>=ContinueRelativePos.x+DialogPos.x&&MousePos.x<=ContinueRelativePos.x+DialogPos.x+ButtonSize.x&&
            MousePos.y>=ContinueRelativePos.y+DialogPos.y&&MousePos.y<=ContinueRelativePos.y+DialogPos.y+ButtonSize.y
        ){
            myMouse.LClickProtection=0;
            yesButton.setColor(col);
            GameActivated = 0;
            screenAdventure.Reset();//////////////////////////////////////////yes
            uis.isSave = 1;
            uis.PauseButtonState = UIState::State::display;
            uis.Dialog_SaveState = UIState::State::hidden;

        }
        if (myMouse.LButtonPressed&&myMouse.LClickProtection&& 
            MousePos.x>=ExitRelativePos.x+DialogPos.x&&MousePos.x<=ExitRelativePos.x+DialogPos.x+ButtonSize.x&&
            MousePos.y>=ExitRelativePos.y+DialogPos.y&&MousePos.y<=ExitRelativePos.y+DialogPos.y+ButtonSize.y
        ){
            myMouse.LClickProtection=0;
            noButton.setColor(col);
            GameActivated = 0;///////////////////////////////////////////no
            screenAdventure.Reset();
            uis.PauseButtonState = UIState::State::display;
            uis.Dialog_SaveState = UIState::State::hidden;
        }
        window.draw(dl);
        window.draw(yesButton);
        window.draw(noButton);
    }
}




std::string PauseButton::ImgPath = "/img/UI/Dialog/Button.png";
PauseButton::PauseButton(COORD xx,COORD yy):x(xx),y(yy),SizeFactor(0.6){
}
void PauseButton::Init(){
    if (!texture.loadFromFile(ParentPath+ImgPath)){
        std::cout<<"Load PauseButton err\n";
    }
    Sizex=texture.getSize().x;
    Sizey=texture.getSize().y;
        
        sf::RenderTexture RT({Sizex,Sizey});    
        sf::Font font;
        if (!font.openFromFile(ParentPath+"/font/arial.ttf")){
            std::cout<<"Failed to load font\n";
        }
        sf::Text Text(font,"pause",Sizey*0.6);
        Text.setFillColor(sf::Color::Green);
    
        sf::Vector2f  TextSize= Text.getGlobalBounds().size;
        
        Text.setPosition({(Sizex-TextSize.x)/2.0,(Sizey-TextSize.y)*0.333});
        
        sf::Sprite Button(texture);
        RT.draw(Button);
        
        RT.draw(Text);
        RT.display();
        
        texture=RT.getTexture();
        Sizex*=SizeFactor;Sizey*=SizeFactor;
}
void PauseButton::update(UIState& uis){
    if (uis.PauseButtonState==UIState::State::display){  ///Solo se ejecuta si el botón está en estado visible (display) dentro del UIState.
        sf::Sprite sp(texture);
        sp.setScale({SizeFactor,SizeFactor});
        sp.setPosition({x,y});
        sf::Vector2i MousePos=myMouse.MousePosition;
        if (myMouse.LButtonPressed&&myMouse.LClickProtection&& 
            MousePos.x>=x&&MousePos.x<=x+Sizex&&
            MousePos.y>=y&&MousePos.y<=y+Sizey)
        { // si El botón izquierdo del mouse está presionado. No se ha registrado un clic recientemente(LClickProtection evita múltiples clics accidentales). El clic está dentro del área del botón.
            uis.Dialog_ExitState= UIState::State::display;
            uis.PauseButtonState = UIState::State::hidden;
            GamePaused = 1;
            
        } //Se muestra un diálogo de salida/pausa (Dialog_ExitState visible). Se oculta el botón de pausa. Se pausa el juego (GamePaused = 1).
        window.draw(sp);
    }
}
ScreenAdventure::ScreenAdventure(COORD xx,COORD yy):Pos(sf::Vector2u({xx,yy})){}

void ScreenAdventure::Init(){
    if (!Texture.loadFromFile(ParentPath+ImgPath))
        std::cout<<"Load err\n";
    if (!StartTexture.loadFromFile(ParentPath+StartImgPath))
        std::cout<<"Load err\n";
    short x=Pos.x,y=Pos.y;

    state = Origin;

    ///////顺时针，判断内部在右手
    Points[0] = sf::Vector2f({x+6*SizeFactor,y+34*SizeFactor});
    Points[1] = sf::Vector2f({x+322*SizeFactor,y+58*SizeFactor});
    Points[2] = sf::Vector2f({x+310*SizeFactor,y+145*SizeFactor});
    Points[3] = sf::Vector2f({x+3*SizeFactor,y+96*SizeFactor});
    // std::cout<<"+==============================+\n";
    // std::cout<<x+6*SizeFactor<<" "<<y+34*SizeFactor<<"\n";
    // std::cout<<x+322*SizeFactor<<" "<<y+58*SizeFactor<<"\n";
    // std::cout<<x+310*SizeFactor<<" "<<y+145*SizeFactor<<"\n";
    // std::cout<<x+3*SizeFactor<<" "<<y+96*SizeFactor<<"\n";

    TickToGetIn = SecToGetIn * TickPerSec;
    IsInPolygon = 0;//必须设置为0，因为鼠标左键未按下时没有更新判断
}

void ScreenAdventure::Reset(){
    TickToGetIn = SecToGetIn * TickPerSec;
    IsInPolygon = 0;//必须设置为0，因为鼠标左键未按下时没有更新判断
}

void ScreenAdventure::update(UIState& uis){
    sf::Sprite* sp;
    if (state == Start){
        sp = new sf::Sprite(StartTexture);
    }else{
        sp = new sf::Sprite(Texture);
    }
    sp->setScale({SizeFactor,SizeFactor});
    sp->setPosition({Pos.x,Pos.y});
    if (myMouse.MouseIsEnter&&myMouse.LButtonPressed&&myMouse.LClickProtection){
        short MouseX=myMouse.MousePosition.x,MouseY=myMouse.MousePosition.y;
        std::cout<<MouseX<<" "<<MouseY<<std::endl;
        std::cout<<TickToGetIn<<std::endl;
        IsInPolygon = 1;
        //诱导顶定向    //////////叉乘  vec1 x vec2
        sf::Vector2f vec1,vec2;
        for (short i=0;i<4;i++){
            vec2=sf::Vector2f({Points[(i+1)%4].x - Points[i].x , Points[(i+1)%4].y - Points[i].y,});
            vec1=sf::Vector2f({MouseX - Points[i].x,MouseY - Points[i].y});
            if (vec1.x * vec2.y - vec2.x * vec1.y > 0){///////////////因为绘图坐标轴y定竖直向下未正，和右手坐标系相反，所以负数才符合
                IsInPolygon = 0;//如果叉乘结果为正，说明鼠标在多边形外部
                break;
            }else{
                // std::cout<<vec1.x<<" "<<vec1.y<<
                std::cout<<i<<" good\n";
            }
        }
        if (IsInPolygon){
            myMouse.LClickProtection = 0;
        }
    }   
    if (IsInPolygon){//如果鼠标在多边形内
        if (TickToGetIn>0)
            TickToGetIn--;
        if (TickToGetIn>TickPerSec*0.25){
            sp->setColor(sf::Color({128,128,128}));
        }
        if (TickToGetIn==0){
            uis.Dialog_RestartState = UIState::State::display;
        }

    }
    window.draw(*sp);//绘制按钮
    delete sp;
}

void Dialog_Restart::update(UIState& uis){
    if (uis.Dialog_RestartState==UIState::State::display){
        sf::Sprite dl(DialogTexture);
        sf::Vector2f DialogPos((WINDOW_WIDTH-DialogSize.x)/2.0,(WINDOW_HEIGHT-DialogSize.y)/2.0);
        dl.setPosition(DialogPos);

        sf::Sprite loadButton(ContinueTexture);
        loadButton.setScale({ButtonSizeFactor,ButtonSizeFactor});
        loadButton.setPosition({
            ContinueRelativePos.x+DialogPos.x,
            ContinueRelativePos.y+DialogPos.y
        });
        sf::Sprite restartButton(ExitTexture);
        restartButton.setScale({ButtonSizeFactor,ButtonSizeFactor});
        restartButton.setPosition({
            ExitRelativePos.x+DialogPos.x,
            ExitRelativePos.y+DialogPos.y
        });


        sf::Color col=loadButton.getColor();
        col.r*=0.5;
        col.g*=0.5;
        col.b*=0.5;
        sf::Vector2i MousePos = myMouse.MousePosition;
        if (myMouse.LButtonPressed&&myMouse.LClickProtection&& 
            MousePos.x>=ContinueRelativePos.x+DialogPos.x&&MousePos.x<=ContinueRelativePos.x+DialogPos.x+ButtonSize.x&&
            MousePos.y>=ContinueRelativePos.y+DialogPos.y&&MousePos.y<=ContinueRelativePos.y+DialogPos.y+ButtonSize.y
        ){
            myMouse.LClickProtection=0;
            loadButton.setColor(col);/////////////////////////////////////////////////continue
            GameActivated = 1;
            GamePaused = 0;
            // uis.PauseButtonState = UIState::State::display;
            uis.Dialog_RestartState = UIState::State::hidden;
            
        }
        if (myMouse.LButtonPressed&&myMouse.LClickProtection&& 
            MousePos.x>=ExitRelativePos.x+DialogPos.x&&MousePos.x<=ExitRelativePos.x+DialogPos.x+ButtonSize.x&&
            MousePos.y>=ExitRelativePos.y+DialogPos.y&&MousePos.y<=ExitRelativePos.y+DialogPos.y+ButtonSize.y
        ){
            myMouse.LClickProtection=0;
            restartButton.setColor(col);////////////////////////////////////restart
            GameActivated = 1;
            GamePaused = 0;
            // uis.PauseButtonState = UIState::State::display;
            uis.Dialog_RestartState = UIState::State::hidden;
            uis.isRestart = 1;
        }
        window.draw(dl);
        window.draw(loadButton);
        window.draw(restartButton);
    }
}




