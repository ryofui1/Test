#include <SFML/Graphics.hpp>
#include <iostream>
#include "Draw.hpp"
#include "main.hpp"

void Draw(sf::Time deltaTime) {
    window.clear(sf::Color::Cyan);
    window.draw(ground);
    window.draw(player.shape);
    for (auto& enemy : enemyList) {
        enemy.draw(deltaTime.asSeconds()*10);
    }

    // 当たり判定計算したペアを線で描画
    for (const auto& line : collisionLines) {
        sf::Vertex vertices[2] = {
            sf::Vertex(line.first, sf::Color::Magenta),
            sf::Vertex(line.second, sf::Color::Magenta)
        };
        window.draw(vertices, 2, sf::Lines);
    }

    window.display();
}