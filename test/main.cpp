#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

int main() {
    // 创建窗口 (SFML 3.0 兼容方式)
    sf::RenderWindow window(VideoMode({570,320}),"PVZ",Style::Default);
    window.setFramerateLimit(60);
    
    // 加载纹理
    sf::Texture texture;
    Image day("img/Background/Day.jpg");
    Image gp("img/GrassPath.jpg");
    // Image day2("Day2.bmg");
    if (!texture.loadFromImage(day)) {
        std::cerr << "无法加载图片文件!" << std::endl;
        return EXIT_FAILURE;
    }
    

    // 检查图片尺寸
    const auto texSize = texture.getSize();
    if (texSize.x < 570u || texSize.y < 320u) {
        std::cerr << "图片尺寸不足! 需要至少 570x320, 实际为: " 
                 << texSize.x << "x" << texSize.y << std::endl;
        return EXIT_FAILURE;
    }

    // 创建精灵
    sf::Sprite sprite(texture);
    
    // 创建视图 (SFML 3.0 兼容方式)
    sf::View view;
    view.setSize({570.f, 320.f});
    view.setCenter({285.f, 160.f}); // 初始居中
    window.setView(view);

    // 滑动控制
    const float scrollSpeed = 15.0f;
    float currentOffset = 0.f;
    const float maxOffset = static_cast<float>(texSize.x - view.getSize().x);

    // 主循环
    while (window.isOpen()) {
        // 事件处理 (SFML 3.0 兼容方式)
        while (const std::optional event = window.pollEvent()) {
            if (event->is<Event::Closed>()) {
                window.close();
            }
            // else if(const auto* keyPressd = event->getIf<Event::KeyPressed>()){
            //     if (keyPressd->scancode==Keyboard::Scancode::Escape)
            //         window.close();
        }

        // 键盘控制 (SFML 3.0 兼容方式)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            currentOffset = std::max(0.f, currentOffset - scrollSpeed);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            currentOffset = std::min(maxOffset, currentOffset + scrollSpeed);
        }



        // 渲染
        window.clear(sf::Color::Black);
        window.draw(sprite);
        window.display();
    }

    return EXIT_SUCCESS;
    
    
    
}