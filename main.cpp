#include "main.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.hpp"
#include "Operation.hpp"
#include "Draw.hpp"
#include "Enemy.hpp"
#include <list>

void defValue() {
    ground.setSize(sf::Vector2f(ScreenWidth, ScreenHeight-groundY));
    ground.setFillColor(sf::Color::Green);
    ground.setPosition(0.f, groundY);
}

sf::Clock myClock;
sf::RenderWindow window(sf::VideoMode(ScreenWidth, ScreenHeight), "TestGame");
Player player;
sf::RectangleShape ground;
//isDebug
bool isDebug = false;

std::list<Enemy> enemyList = {Enemy(0),Enemy(0),Enemy(0),Enemy(0),};
//Enemy(0), Enemy(0),Enemy(0), Enemy(0)
int main() {
    std::cout << "HelloWorld!" << std::endl;
    std::cout << "tybob8010&ryofui1" << std::endl;
    std::cout << "If you want to debug, please press the B key." << std::endl;

    std::cout << ScreenWidth << std:: endl;

    //std::cout << ScreenWidth << std:: endl;
    
    defValue();

    while (window.isOpen()) {
        //処理時間 deltatime
        sf::Time deltaTime = myClock.restart();

        //Debug用
        collisionLines.clear();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // std::cout << "Operation" << std::endl;
        Operation(deltaTime);
        // std::cout << "Draw" << std::endl;
        Draw(deltaTime);
        // std::cout << "End of frame" << std::endl;
    }


    return 0;
}
