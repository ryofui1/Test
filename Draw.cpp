#include <SFML/Graphics.hpp>
#include <iostream>
#include "Draw.hpp"
#include "main.hpp"

void Draw(sf::Time deltaTime) {
    //std::cout << "Drawing frame" << std::endl;
    window.clear(sf::Color::Cyan);
    window.draw(ground);
<<<<<<< HEAD
    player.draw(deltaTime.asSeconds()*10);
=======
    window.draw(player.shape);
    //std::cout << "Drawing Enemy" << std::endl;
    for (auto& enemy : enemyList) {
        enemy.draw(deltaTime.asSeconds()*10);
    }

    //std::cout << "Drawing collision lines" << std::endl;
    // 当たり判定計算したペアを線で描画
    for (const auto& line : collisionLines) {
        sf::Vertex vertices[2] = {
            sf::Vertex(line.first, sf::Color::Magenta),
            sf::Vertex(line.second, sf::Color::Magenta)
        };
        window.draw(vertices, 2, sf::Lines);
    }

>>>>>>> 05dd983e89aec9df692753419e39bd8ca7b833c6
    window.display();
}