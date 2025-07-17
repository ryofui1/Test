#include <SFML/Graphics.hpp>
#include <iostream>
#include "Draw.hpp"
#include "main.hpp"

void Draw(sf::Time deltaTime) {
    // std::cout << "Drawing frame" << std::endl;
    window.clear(sf::Color::Cyan);
    window.draw(ground);
    // std::cout << "Drawing Player" << std::endl;
    player.draw(deltaTime.asSeconds());

    // std::cout << "Drawing PlayerShape" << std::endl;
    //window.draw(player.shape);

    // std::cout << "Drawing Enemy" << std::endl;
    for (auto& enemy : enemyList) {
        enemy->draw(deltaTime.asSeconds());
    }

    for (auto& object : ObjectList_EnemyAttack) {
        object.draw(deltaTime.asSeconds());
    }
    for (auto& object : ObjectList_PlayerAttack) {
        object.draw(deltaTime.asSeconds());
    }
    for (auto& object : ObjectList_Other) {
        object.draw(deltaTime.asSeconds());
    }

    // std::cout << "Drawing collision lines" << std::endl;
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